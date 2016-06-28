#include<bits/stdc++.h>

#define DataMax 42000000
#define StrMax 831000000
#define HashMult 65537
#define HashMod 9999991
#define HashMax 10000000
#define WordMult 31
#define WordMod 45677

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

char *ori_str = new char[StrMax],*ori_str_cst=ori_str;
char stop='\t';
struct Sentence
{
	char *str;
	unsigned int feq;

	int noprep_n,
		noprep_num[6];// the last one for code easier
	
	int words_n,
		words_pos[11],
		words_len[11];

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

	friend bool sentEq(const Sentence *a,const Sentence *b)
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

	int sentDeal()// modify origin string
	{
		char *c = str;
		
		// get the size of each word
		words_n = 0;
		int start=-1,i;
		for(i=0;c[i]!=stop;++i)
			if( c[i] != ' ')
			{
				if(start==-1)
					start=i;
			}
			else 
			{
				words_pos[words_n  ]=  start;
				words_len[words_n++]=i-start;
				if( words_n >= 11) // the size should no more than 11
					return -1;
				start=-1;
			}
		words_pos[words_n  ]=  start;
		words_len[words_n++]=i-start;

		// check if prepos and get hash
		ll all=0;
		noprep_n=0;
		for(int i=0; i<words_n ; ++i)
		{
			if( !preposCheck(c,words_len[i]) )
			{
				int sum=0;
				for(int j=0;j<words_len[i];++j)
					sum = (sum*WordMult+c[j])%WordMod;
				all =     (all*HashMult+sum )%HashMod;
				if( noprep_n>= 5) // the size should no more than 6 
					return -1;
				noprep_num[noprep_n++]= i;
			}
			c += words_len[i] + 1;
		}
		
		//return hash
		noprep_num[noprep_n]= words_n ; // for code easier
		return (int)all;

	}

}*ori_data = new Sentence [DataMax];

bool sentComp(const Sentence *a,const Sentence *b)
{
	if( a->feq != b->feq )
		return a->feq > b->feq;
	return strcmp( a->str, b->str) < 0 ;
}

bool preposSame(Sentence *a,Sentence *b,int na,int nb)
{
	return a->words_len[na] == b->words_len[nb] &&
		a->str[ a->words_pos[na]   ] == b->str[ b->words_pos[nb]   ] &&
		a->str[ a->words_pos[na]+1 ] == b->str[ b->words_pos[nb]+1 ] ;
}
