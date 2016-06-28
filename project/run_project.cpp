#include "project.cpp"

bool isEd1(Sentence *a,Sentence *b,int &pa,int &pb,int &num)
{
	int lena = a->noprep_num[num] - pa ,
		lenb = b->noprep_num[num] - pb ;//lenb must > 0 
	if( std::abs(lena-lenb)> 1 ) return 0;//quick

	int cona=0,conb=0;//from head and from tail
	while( cona<lena && cona<lenb && preposSame(a,b,pa+cona,pb+cona)) 
		++cona;
	if( lena == lenb && cona == lena )// dis = 0
		return 1; 
	while( conb<lena && conb<lenb && preposSame(a,b,pa+lena-1-conb,pb+lenb-1-conb)) 
		++conb;
	if( lena == lenb && cona+conb == lena-1 ) // dis = 1 sub
		return 1;
	if( lena != lenb && cona+conb >= std::min(lena,lenb) ) // dis = 1 add del
		return 1;  // larger beacuse it can be like  aa aa , aa
	return 0; 
}

bool addEd2(Sentence *a,Sentence *b)// b is input
{
	if( a->words_n < b->words_n || a->words_n > b->words_n+2)
		return 0;
	int nowa=0,nowb=0;
	int dis=0;
	for(int num=0;num<=b->noprep_n && dis>2;++num)
	{
		for( nowb;nowb<b->noprep_num[num];++nowb)
		{
			for(nowa ; nowa < a->noprep_num[num] ;++nowa)
				if( preposSame(a,b,nowa,nowb) )
					break;
				else 
					++dis;
			if( nowa == a->noprep_num[num] )
				return 0; // can't find match one
			++nowa;
		}
		dis += a->noprep_num[num] - nowa;
		nowa = a->noprep_num[num] + 1   ;
		nowb = b->noprep_num[num] + 1   ;
	}
	return dis<=2 ;
}

bool allEd1(Sentence *a,Sentence *b)
{
	int nowa=0,nowb=0;
	for(int num=0;num<=b->noprep_n;++num)
	{
		if(    nowb == b->noprep_num[num] //no operator between
			&& nowa != a->noprep_num[num] )
				return 0;
		if( !isEd1(a,b,nowa,nowb,num) )
			return 0;
		nowa = a->noprep_num[num] + 1   ;
		nowb = b->noprep_num[num] + 1   ;
	}
	return 1;
}

std::vector<Sentence *> ans;
int      hash_len  [HashMax];
int      hash_start[HashMax];
int      hash_size [DataMax];
long int hash_where[DataMax];
int 	 sentsize =  sizeof(Sentence);
char *ori_str_old ;
FILE *file ;

void fileTodata(char *filename)
{
	FILE *f = fopen(filename,"rb");
	fseek(f,0,SEEK_SET);
	int sizelen,datalen;
	fread(&sizelen ,sizeof(int),1      ,f);
	fread(&datalen ,sizeof(int),1      ,f);
	fread(hash_len ,sizeof(int),HashMax,f);
	fread(hash_size,sizeof(int),sizelen,f);

	//start
	hash_start[0]=0;
	for(int i=1;i<HashMax;++i)
		hash_start[i] = hash_start[i-1] + hash_len[i-1];

	//where
	hash_where[0]=ftell(f);
	for(int i=1;i<sizelen;++i)
		hash_where[i] = hash_where[i-1] + (long int)hash_size[i-1]*sentsize;
	fseek(f,(long int)datalen*sentsize,SEEK_CUR);

	// read str
	fread(&ori_str_old,sizeof(char*),1     ,f);
	fread( ori_str    ,sizeof(char ),StrMax,f);
	fclose(f);
	file = fopen(filename,"rb");
}

int whereFind(int hash,Sentence *s)
{
	for(int i=0 ; i<hash_len[hash] ; ++i)
	{
		Sentence sent ;
		fseek(file,hash_where[ hash_start[hash]+i ],SEEK_SET);
		fread(&sent,sentsize,1,file);
		sent.str = ori_str + ( sent.str - ori_str_old); 
		if( sentEq( &sent, s ) ) 
			return hash_start[hash]+i;
	}
	return -1;
}

void sentFilt(int where,Sentence *s,bool(*check)(Sentence *,Sentence *))
{
	int size = hash_size [ where ];
	fseek(file,hash_where[ where ],SEEK_SET);
	fread(ori_data,sentsize,size,file);
	for(int i=0;i<size ;++i)
	{
		ori_data[i].str = ori_str + ( ori_data[i].str - ori_str_old);
		if( check( ori_data+i , s) )
		{
			ans.push_back( ori_data+i );
			if( ans.size() >= 10)
				break;
		}
	}
}

int main(int argc,char *argv[])
{
	if(argc!=2)
		puts("arguments error");
	preposInit();
	fileTodata(argv[1]);
	stop = '\n';
	Sentence *s = new Sentence;
	s->str = new char[500];
	while( fgets(s->str,500,stdin) )
	{
		printf("query: %s",s->str);
		int h = s->sentDeal();
		if(h==-1)// for too big to array
		{
			printf("output: 0\n");
			continue;
		}
		int where = whereFind(h,s);
		ans.clear();

		if( s->words_n == s->noprep_n )//no prep  
			sentFilt( where, s, addEd2);
		else
			sentFilt( where, s, allEd1);
		
		// query
		printf("output: %d\n",ans.size());
		for(int i=0;i<ans.size();++i)
			printf("%s\t%u\n",ans[i]->str,ans[i]->feq);
	}
	return 0;
}

