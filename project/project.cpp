#include<bits/stdc++.h>

#define DataMax 50000000
#define HashMult 2350001
#define HashMod 99999989
#define WordMult 11261
#define WordMod 46301

#define ll long long 
using std::string;

struct Sentence
{
	char str[220];
	int feq;
}*ori_data = new Sentence [DataMax];
int ori_n=0;

bool sentComp(Sentence *a,Sentence *b)
{
	if( a->feq != b->feq)
		return a->feq > b->feq;
	else
		return a < b;
}

struct Link
{
	Sentence *str;
	Link *next;
}*nextp =new Link[DataMax],*hash_table[100000000];

int sentHash(char *&c)// modify origin string
{
	int i,sum=0;
	ll all=0;

	for(i=0;c[i]!='\t';++i)
	{
		if( c[i] != ' ' )
			sum = (sum*WordMult+c[i])%WordMod;
		else 
		{
			all = (all*HashMult+sum )%HashMod;
			sum = 0;
		}
	}
	all = (all*HashMult+sum )%HashMod;
	c = c+i;
	return (int)all;
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
			char *c = ori_data->str;
			int h = sentHash(c);

			// get fre and modify
			*(c++) = '\0';
			int dig=0;
			for(int i=0;c[i]!='\n';++i)
				dig = dig*10 + c[i]-'0';
			ori_data->feq = dig;

			// put into hashtable
			nextp->str  = ori_data++;
			nextp->next = hash_table[h];
			hash_table[h]= nextp++ ;
		}
		fclose(f);
	}
}

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

	bool operator == (const char *c) const
	{
		int k=0;
		for(int i=0;i<len;++i)
		{
			for(int j=0;word[i].str[j];++j)
				if(word[i].str[j]!=c[k++])
					return false;
			if(i!=len-1 && c[k++]!=' ')
				return false;
		}
		return c[k]=='\0';
	}

};

Sentence* findInhashtable(Sepword &s)
{
	Link *now = hash_table[s.hash];
	while( now )
	{
		if( s == now->str->str)
			return now->str;
		now = now->next;
	}
	return NULL;
}

std::vector<Sentence*> ans;
void findToans(Sepword &s)
{
	Sentence* getsent = findInhashtable(s);
	if( getsent)
		ans.push_back(getsent);
}

void onlyAdd(Sepword &s,int dis=0)
{
	findToans(s);
//	s.allPrint();
	if(s.len==0 || dis==2) return ;
	ll hm = 1;
	ll h= s.hash;

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
				onlyAdd(s,dis+1);
			}
		hm = hm * HashMult % HashMod;
	}
	for(int i=0;i<s.len;++i)
		s.word[i] = s.word[i+1] ;
	--s.len;
}

void edMake(Sepword &s)
{
	findToans(s);
//	s.allPrint();
	if(s.len==0 ) return ;
	Word tmp;
	ll hm = 1;
	ll h= s.hash;
	
	//delete
	--s.len;
	for(int i=s.len;i>=0;--i)
	{
		wordSwap( tmp , s.word[i] );
		s.hashGet();
		if(tmp.isprepos)
			findToans(s);
//			edMake(s,dis+1);
	}
	for(int i=0;i<=s.len;++i)
		wordSwap( tmp , s.word[i] );
	++s.len;

	//substitude
	for(int i=s.len-1;i>=0;--i)
	{
		h = ((h-hm*s.word[i].hash)%HashMod+HashMod)%HashMod;
		tmp = s.word[i] ;
		if(tmp.isprepos)
			for(int j=0;j<prepos_n;++j)
			{
				s.word[i] = prepos_v[j];
				s.hash = (h+(ll)s.word[i].hash*hm)%HashMod;
				findToans(s);
//				edMake(s,dis+1);
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
		if( (i && s.word[i-1].isprepos) || (i+1<s.len && s.word[i+1].isprepos))
			for(int j=0;j<prepos_n;++j)
			{
				s.word[i] = prepos_v[j];
				s.hash = (h+hm*s.word[i].hash)%HashMod;
				findToans(s);
//				edMake(s,dis+1);
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

void ansPrint()
{
	std::sort(ans.begin(),ans.end(),sentComp);
	if(ans.size()==0)
	{
		puts("output: 0");
		return ;
	}
	int n=1;
	for(int i=1;i<ans.size() && n<10;++i)// clear the same item
		if( ans[i-1] != ans[i])
			ans[n++] = ans[i];
	if(n>10)
		n=10;
	printf("output: %d\n",n);
	for(int i=0;i<n;++i)
		printf("%s\t%d\n",ans[i]->str,ans[i]->feq);
}

bool hasprepos(Sepword &s)
{
	for(int i=0;i<s.len;++i)
		if(s.word[i].isprepos)
			return 1;
	return 0;
}
	
int main()
{
	preposInit();
	fileTohashtable();
	char c[1000];
	while( fgets(c,100,stdin) )
	{
		printf("query: %s",c);
		Sepword s;
		s.inputSep(c);
		ans.clear();

		// check if it has prepos
		if(hasprepos(s))
			edMake(s);
		else
			onlyAdd(s,0);

		ansPrint();
		
	}
	return 0;
}

