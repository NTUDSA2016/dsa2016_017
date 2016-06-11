#include<bits/stdc++.h>

#define DataMax 42000000
//#define DataMax 500
#define HashMult 65537
#define HashMod 99999989
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

char stop='\t';
struct Sentence
{
	char str[220];
	unsigned int feq;

	int noprep_n,
		noprep_num[6];
	
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
				noprep_num[noprep_n++]= i;
				if( noprep_n>= 6) // the size should no more than 6 
					return -1;
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
			int h = ori_data->sentDeal();
			
			// get feq
			char *c=&ori_data->str[ori_data->words_pos[ ori_data->words_n-1 ]+
					               ori_data->words_len[ ori_data->words_n-1 ]];
			*(c++) = '\0';
			unsigned int dig=0;
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

bool allEd1(Sentence *a,Sentence *b)
{
	int preva= 0, prevb= 0 ;
//	a->noprep_num[ a->noprep_n ]= a->words_n;  i add before
	for(int num=0;num<=b->noprep_n;++num)
	{
		if(    prevb == b->noprep_num[num] //no operator between
			&& preva != a->noprep_num[num] )
				return 0;
		if( !isEd1(a,b,preva,prevb,num) )
			return 0;
		preva= a->noprep_num[num]+1;
		prevb= b->noprep_num[num]+1;
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

void hashCollide()
{
	std::map<int,int> map;
	for(int i=0;i<100000000;++i)
	{
		Linkhash *lkh = hash_table[i];
		int len=0;
		while( lkh )
		{
			++len;
			lkh = lkh->next;
		}
		map[len]++;
	}

	for(auto &i:map)
		printf("%d %d\n",i.first,i.second);
}


int main()
{
	preposInit();
	fileTohashtable();
//	hashCollide(); return 0;
//	return 0;
	stop = '\n';
	Sentence *s = new Sentence;
	while( fgets(s->str,220,stdin) )
	{
		printf("query: %s",s->str);
		int h = s->sentDeal();
		if(h==-1)// for too big to array
		{
			printf("output: 0\n");
			continue;
		}
		Linkhash *lkh = linkFind(h,s);
		ans.clear();

		if( s->words_n == s->noprep_n )//no prep  
			sentFilt( lkh, s, addEd2);
		else
			sentFilt( lkh, s, allEd1);
		
		std::sort(ans.begin(),ans.end(),sentComp);
		if(ans.size()>10)
			ans.resize(10);
		// query
		printf("output: %d\n",ans.size());
		for(int i=0;i<ans.size();++i)
			printf("%s\t%u\n",ans[i]->str,ans[i]->feq);
	}
	return 0;
}

