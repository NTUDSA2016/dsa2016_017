#include<bits/stdc++.h>

#define DataMax 50000000
//#define DataMax 500
#define HashMult 2350001
#define HashMod 99999989
#define WordMult 11261
#define WordMod 46301

#define ll long long 
using std::string;

struct Trie
{
	bool exist;
	Trie* ch[26];
	Trie():exist(0){
		memset(ch,0,sizeof(ch));
	}


}*root;

void preposInsert(char *str)
{
	Trie *now = root;
	for(int i=0;str[i];++i)
	{
		char c = str[i]-'a';
		if( now->ch[c] )
			now = now->ch[c];
		else
			now = now->ch[c] = new Trie;
	}
	now->exist=1;
}

bool preposCheck(char *str,int& len)
{
	Trie *now = root;
	for(int i=0;i<len;++i)
	{
		now = now->ch[str[i]-'a'];
		if(!now) return 0;
	}
	return now->exist;
}

void preposInit()
{
	const int prepos_n = 20;
	char prepos_c[prepos_n][10]= {
		"of", "to", "in", "for", "with", 
		"on", "at","by", "from", "up", 
		"about", "than", "after", "before", "down", 
		"between", "under", "since", "without", "near"};

	root = new Trie;
	for(int i=0;i<prepos_n;++i)
		preposInsert(prepos_c[i]);
}

struct Sentence
{
	char str[220];
	int feq;

	int noprep_n,
		noprep_num[6];
	
	int words_n,
		words_pos[10],
		words_len[10];

	void sentPrint()
	{
		puts("----------------------");
		printf("n%d\n",noprep_n);
		for(int i=0;i<words_n;++i)
			printf("%d %d\n",words_pos[i],words_len[i]);
		for(int i=0;i<noprep_n;++i)
			printf("%d\n",noprep_num[i]);
		printf("%s\t%d\n",str,feq);
	}


}*ori_data = new Sentence [DataMax];
int ori_n;

bool sentEq(const Sentence *a,const Sentence *b)
{
	if(a->noprep_n != b->noprep_n)
		return 0;
	for(int i=0;i<a->noprep_n;++i)
	{
		if(a->words_len[a->noprep_num[i]]!=b->words_len[b->noprep_num[i]])
			return 0;
		int len = a->words_len[a->noprep_num[i]];
		int pos = a->words_pos[a->noprep_num[i]],
			posb= b->words_pos[b->noprep_num[i]];
		for(int j=0;j<len;++j)
			if(a->str[pos+j] != b->str[posb+j])
				return 0;
	}
	return 1;
}

bool sentComp(const Sentence *a,const Sentence *b)
{
	return a->feq > b->feq;
}

char stop='\t';
int sentDeal(Sentence *s)// modify origin string
{
	char *c = s->str;
	
	// get the size of each word
	s->words_n = 0;
	int start=-1,i;
	for(i=0;c[i]!=stop;++i)
		if( c[i] != ' ')
		{
			if(start==-1)
				start=i;
		}
		else 
		{
			s->words_pos[s->words_n  ]=  start;
			s->words_len[s->words_n++]=i-start;
			start=-1;
		}
	s->words_pos[s->words_n  ]=  start;
	s->words_len[s->words_n++]=i-start;

	// check if prepos and get hash
	ll all=0;
	s->noprep_n=0;
	for(int i=0; i<s->words_n ; ++i)
	{
		if( !preposCheck(c,s->words_len[i]) )
		{
			int sum=0;
			for(int j=0;j<s->words_len[i];++j)
				sum = (sum*WordMult+c[j])%WordMod;
			all = (all*HashMult+sum )%HashMod;
			s->noprep_num[s->noprep_n++]= i;
		}
		c += s->words_len[i] + 1;
	}
	
	//return hash
	s->noprep_num[s->noprep_n]= s->words_n ; // for code easier
	return (int)all;

}

struct Link
{
	Sentence *sent;
	Link *next;
}*nextlk =new Link[DataMax];

struct Linkhash
{
	Link *lk;
	Linkhash *next;
}*nextlkh =new Linkhash[DataMax],*hash_table[100000000];

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
		while( fgets(ori_data->str,220,f) )
		{
			++ori_n;
			int h = sentDeal(ori_data);
			
			// get feq
			char *c=&ori_data->str[ ori_data->words_pos[ ori_data->words_n-1 ]+
					               ori_data->words_len[ ori_data->words_n-1 ]];
			*(c++) = '\0';
			int dig=0;
			for(int i=0;c[i]!='\n';++i)
				dig = dig*10 + c[i]-'0';
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

std::vector< Sentence * > ans;

bool preposSame(Sentence *a,Sentence *b,int na,int nb)
{
	return a->words_len[na] == b->words_len[nb] &&
		a->str[ a->words_pos[na]   ] == b->str[ b->words_pos[nb]   ] &&
		a->str[ a->words_pos[na]+1 ] == b->str[ b->words_pos[nb]+1 ] ;
}

bool isEd2(Sentence *a,Sentence *b)// b is input
{
	if( a->words_n < b->words_n || a->words_n > b->words_n+2)
		return 0;
	int nowa=0,nownum=0;
	int dis=0;
	for(int nowb=0;nowb<b->words_n && dis>2;++nowb)
		if( nowb == b->noprep_num[nownum] )// a is more than b
		{
			dis += a->noprep_num[nownum] - nowa;
			nowa = a->noprep_num[nownum]+1;
			++nownum;
		}
		else
		{
			// find if b can match to a exclude add one
			for(nowa ; nowa < a->noprep_num[nownum] ;++nowa)
				if( preposSame(a,b,nowa,nowb) )
					break;
				else 
					++dis;
			if( nowa == a->noprep_num[nownum] )
				return 0; // can't find match one
			++nowa;
		}

	return dis<=2 ;
}

bool canInsert(Sentence *a,Sentence *b)
{
	int preva= -1, prevb= -1 ;
//	a->noprep_num[ a->noprep_n ]= a->words_n;  i add before
	for(int num=0;num<=b->noprep_n;++num)
	{
		if(    prevb +1 == b->noprep_num[num] //no operator between
			&& preva +1 != a->noprep_num[num] )
				return 0;
		preva= a->noprep_num[num];
		prevb= b->noprep_num[num];
	}
	return 1;
}

void sentFilt(Linkhash *lkh,Sentence *s,bool(*check)(Sentence *,Sentence *))
{
	if(!lkh)return ;
	Link *lk = lkh->lk ;
	while( lk )
	{
		if( check( lk->sent, s) )
			ans.push_back(lk->sent);
		lk = lk->next;
	}
}

int main()
{
	preposInit();
	fileTohashtable();
	stop = '\n';
	Sentence *s = new Sentence;
	while( fgets(s->str,220,stdin) )
	{
		int h = sentDeal(s);
		Linkhash *lkh = linkFind(h,s);
		ans.clear();

		if( s->words_n == s->noprep_n )//no prep  
			sentFilt( lkh ,s ,isEd2	   );
		else
			sentFilt( lkh ,s ,canInsert);
		
		std::sort(ans.begin(),ans.end(),sentComp);
		if(ans.size()>10)
			ans.resize(10);
		// query
		printf("query: %s",s->str);
		printf("output: %d\n",ans.size());
		for(int i=0;i<ans.size();++i)
			printf("%s\t%d\n",ans[i]->str,ans[i]->feq);
	}
	return 0;
}

