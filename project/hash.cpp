#include<bits/stdc++.h>

#define HashMult 2350001
#define HashMod 99999989
#define WordMult 11261
#define WordMod 46301
#define wordLen 40
#define ll long long 
using std::string;

const int prepos_n=20;
std::unordered_set<std::string> prepos_s;
//Word prepos_v[prepos_n]; i can't put it here QQ

struct Word // this is use for shallow copy  be aware!!
{
	char *str,isprepos;
	int  hash;
	int  len ;
	Word():str(NULL){};
	~Word()
	{
		if(str) 
			delete[] str;
	}

	void wordNew(char *c,int l)
	{
		len = l;
		hash=0;
		
		str = new char[len+1];
		for(int i=0;i<len;++i)
		{
			hash = (hash*WordMult+c[i])%WordMod;
			str[i] = c[i];
		}
		str[len]='\0';
		isprepos = (char)prepos_s.count(str);
	}
};

Word prepos_v[prepos_n];

void wordSwap(Word &a,Word &b)
{
	Word c;
	c=a;a=b;b=c;
	c.str = NULL; //avoid unwanted deletion
}

struct Sepword
{
	Word word[8];
	int  len,hash;
	
	void allPrint()
	{
		for(int i=0;i<len;++i)
			printf("%s:",word[i].str);
		puts("");
//		for(int i=0;i<len;++i)
//			printf("%d:",word[i].len);
//		puts("");
		for(int i=0;i<len;++i)
			printf("%d:",word[i].hash);
		puts("");
//		for(int i=0;i<len;++i)
//			printf("%d:",word[i].isprepos);
//		puts("");
		printf("%d\n",hash);
		printf("%d\n",hashGet());
	}
	
	int hashGet()
	{
		ll h=0;
		for(int i=0;i<len;++i)
			h = (h*HashMult+word[i].hash)%HashMod;
		return hash = (int)h;
	}
	
	void inputSep(char *c)
	{
		len=0;
		int start=-1;
		for(int i=0;c[i];++i)
		{
			if( !isalpha(c[i]) )
				continue;
			int start=i;
			while( isalpha(c[i]) )
				++i;
			word[len++].wordNew(c+start,i-start);
			if(!c[i])break;// be careful for last one
		}
		hashGet();
	}

};



void edMake(Sepword &s,int dis=0)
{
	if(s.len==0) return ;
	s.allPrint();
	if(dis==2) return ;
	Word tmp;
	ll hm = 1;
	ll h= s.hash;
	
	//delete
	--s.len;
	for(int i=s.len;i>=0;--i)
	{
		wordSwap( tmp , s.word[i] );
		s.hashGet();
//		if(tmp.isprepos)
			edMake(s,dis+1);
	}
	for(int i=0;i<=s.len;++i)
		wordSwap( tmp , s.word[i] );
	++s.len;

	//substitude
	for(int i=s.len-1;i>=0;--i)
	{
		h = ((h-hm*s.word[i].hash)%HashMod+HashMod)%HashMod;
		tmp = s.word[i] ;
//		if(tmp.isprepos)
			for(int j=0;j<prepos_n;++j)
			{
				s.word[i] = prepos_v[j];
				s.hash = (h+(ll)s.word[i].hash*hm)%HashMod;
//				s.hashGet();
				edMake(s,dis+1);
			}
		s.word[i] = tmp ;
		h = (h+hm*s.word[i].hash)%HashMod;
		hm = hm * HashMult % HashMod;
	}
	tmp.str = NULL; //avoid unwanted deletion

	//add
	if( s.len>=7)
		return;
	++s.len;
	hm=1;
	for(int i=s.len-1;i>=0;--i)
	{
		s.word[i+1] = s.word[i] ;
		s.word[i].hash=0;
		h = s.hashGet();
//		if( (i && s.word[i-1].isprepos) || (i+1<s.len && s.word[i+1].isprepos))
			for(int j=0;j<prepos_n;++j)
			{
				s.word[i] = prepos_v[j];
				s.hash = (h+hm*s.word[i].hash)%HashMod;
//				s.hashGet();
				edMake(s,dis+1);
			}
		hm = hm * HashMult % HashMod;
	}
	for(int i=0;i<s.len;++i)
		s.word[i] = s.word[i+1] ;
	--s.len;
}

void preposInit()
{
	char prepos_c[prepos_n][10]= {
		"of", "to", "in", "for", "with", 
		"on", "at","by", "from", "up", 
		"about", "than", "after", "before", "down", 
		"between", "under", "since", "without", "near"};

	for(int i=0;i<prepos_n;++i)
	{
		prepos_v[i].wordNew(prepos_c[i],strlen(prepos_c[i]));
		prepos_s.insert(prepos_c[i]);
	}
}

int main()
{
	preposInit();
	char c[1000];
	while( fgets(c,100,stdin) )
	{
		Sepword s;
		s.inputSep(c);
//		s.allPrint();
		edMake(s,0);
		s.allPrint();
	}
	return 0;
}

	
