#include "project.cpp"

struct Link
{
	Sentence *sent;
	Link *next;
} *nextlk   = new Link     [DataMax];
struct Linkhash
{
	Link *lk;
	Linkhash *next;
} *nextlkh  = new Linkhash [DataMax],*hash_table[HashMax];

Linkhash* linkFind(int &hash,Sentence *s)
{
	Linkhash *lkh = hash_table[hash];
	while(lkh)
	{
		if( sentEq(lkh->lk->sent , s) )
			return lkh;
		lkh = lkh->next;
	}
	return NULL;
}


void fileTohashtable()
{
	// hashtable will be initized with 0
	char filename[220];
	for(int name=2;name<=5;++name)
	{
		sprintf(filename,"/tmp2/dsa2016_project/%dgm.small.txt",name);
		FILE *f = fopen(filename,"r");
		while( fgets(ori_str,220,f) )
		{
			ori_data->str = ori_str;
			int h = ori_data->sentDeal();
			
			// get feq
			ori_str += ori_data->words_pos[ ori_data->words_n-1 ]+
			           ori_data->words_len[ ori_data->words_n-1 ];
			*(ori_str++) = '\0';
			unsigned int dig=0;
			for(int i=0;ori_str[i]!='\n';++i)
				dig = dig*10 + ori_str[i]-'0';
			ori_data->feq = dig;
			
			// put into hashtable
			Linkhash *lkh = linkFind(h,ori_data);
			if( !lkh )
			{
				lkh = nextlkh;
				nextlkh->next = hash_table[h] ;
				hash_table[h] = nextlkh++ ;
			}
				
			nextlk->sent  = ori_data++;
			nextlk->next = lkh->lk ;
			lkh->lk = nextlk++ ;
		}
		fclose(f);
	}
}

void dataTofile(char *filename)
{
	FILE *f = fopen(filename,"wb");

	Sentence **tmp = new Sentence* [DataMax];

	int *hash_len = new int [HashMax];
	int *hash_size= new int [DataMax];

	int tmplen=0,sizelen=0;
	
	int sentsize = sizeof(Sentence);
	for(int i=0;i<HashMax;++i)
	{
		int len=0;
		Linkhash *lkh = hash_table[i];
		while( lkh )
		{
			++len;
			Link *lk = lkh->lk;
			int start=tmplen;
			while( lk )
			{
				tmp[tmplen++] = lk->sent;
				lk = lk->next;
			}
			std::sort(tmp+start,tmp+tmplen,sentComp);
			hash_size[sizelen++] = tmplen-start;
			lkh = lkh->next;
		}
		hash_len[i] = len;
	}
	fwrite(&sizelen ,sizeof(int),1      ,f);
	fwrite(hash_len ,sizeof(int),HashMax,f);
	fwrite(hash_size,sizeof(int),sizelen,f);
	for(int i=0;i<tmplen;++i)
		fwrite(tmp[i],sentsize,1,f);
	fwrite(&ori_str_cst,sizeof(char*),1     ,f);
	fwrite( ori_str_cst,sizeof(char ),StrMax,f);
	fclose(f);
}


int main(int argc,char *argv[])
{
	if(argc!=2)
		puts("arguments error");
	preposInit();
	fileTohashtable();
	dataTofile(argv[1]);
	return 0;
}
