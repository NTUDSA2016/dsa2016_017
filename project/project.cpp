#include<bits/stdc++.h>

#define wordLen 40
#define ll long long 
using std::string;

struct word
{
	char str[220];
	int feq;
}ori_data[5774000];

struct link
{
	word *str;
	link *next;
}*nextp,*hashTable[100000000];

#define Hash_mult 2350001
#define Hash_mod 99999989

int Gohash(int *c)//end with -1
{
	ll sum=0;
	for(int i=0;c[i]!=-1;++i)
		sum = (sum*Hash_mult+c[i])%Hash_mod;
	return (int)sum;
}
int Gohash(int *c,int n)//size n
{
	ll sum=0;
	for(int i=0;i<n;++i)
		sum = (sum*Hash_mult+c[i])%Hash_mod;
	return (int)sum;
}

#define Word_mult 11261
#define Word_mod 46301
// should not bigger than 46340


//not good . put it outside
int tmp_words[10];
int tmp_end;
int WordHash(char *c)
{
	int sum=0,len=0,start=-1;
	
	for(int i=0;c[i];++i)
	{
		if( isalpha(c[i]) )
		{
			if( start==-1 )
				start = i ;
			sum = (sum*Word_mult+c[i])%Word_mod;
		}
		else if( start !=-1)
		{
//			for(int j=start;j<i;++j)
//				printf("%c",c[j]);
//			puts("");
			tmp_words[len++] = sum;
			sum = 0;
			start=-1;
			tmp_end = i ;
		}
	}
	tmp_words[len++] = -1;
	return Gohash(tmp_words);
}

void Filetohashtable()
{
	// hashtable will be initized with 0
	nextp = new link[5774000];
	FILE *f = fopen("5gm.txt","r");
	int len=0;
	while( fgets(ori_data[len].str,220,f) )
	{
		char *c = ori_data[len].str;
		int h = WordHash(c);

		// modify the string
		int dig=0,&i=tmp_end;
		c[i]='\0';
		for(;!isdigit(c[i]);++i);
		for(;isdigit(c[i]);++i)
			dig = dig*10 + c[i]-'0';
		ori_data[len].feq = dig;

		// put into hashtable
		nextp->str  = ori_data+len;
		nextp->next = hashTable[h];
		hashTable[h]= nextp++ ;
		++len;
	}
	fclose(f);
}

std::vector<word*> ans;
bool wordcomp(word *a,word *b)
{
	return a->feq > b->feq;
}

#define questionN 20
char candi[questionN][10]= {"of", "to", "in", "for", "with", "on", "at","by", "from", "up", "about", "than", "after", "before", "down", "between", "under", "since", "without", "near"};
int hash_cadi[questionN];


struct Sepword
{
	int n;
	char *str[10];// easy for change
	int h[10],hash;
	Sepword()
	{
		n=0;
	}
	~Sepword()
	{
		for(int i=0;i<n;++i)
			delete[] str[i];
	}

	void print()
	{
		for(int i=0;i<n;++i)
			printf("%s ",str[i]);
		puts("");
		for(int i=0;i<n;++i)
			printf("%d ",h[i]);
		puts("");
	}

	inline void swap(int a,int b)
	{
		std::swap(str[a],str[b]);
		std::swap(  h[a],  h[b]);
	}
	inline void candi_in(int i,int c)
	{
		str[i] = candi[c];
		  h[i] = hash_cadi[c];
	}


	bool operator == (const char *c) const
	{
		int k=0;
		for(int i=0;i<n;++i)
		{
			for(int j=0;str[i][j];++j)
				if(str[i][j]!=c[k++])
					return false;
			if(i!=n-1 && c[k++]!=' ')
				return false;
		}
		return true;
	}

	word* getfromhash()
	{
		link *now = hashTable[hash];
		while( now )
		{
			if( *this == now->str->str)
				return  now->str;
			now = now->next;
		}
		return NULL;
	}

	void put_to_answer()// it can be impoved
	{
		word *str = getfromhash();
		if(str)
			ans.push_back(str);
	}

	void input_sep(char *c)
	{
		int j=0;
		n=0;
		for(int i=0;c[i];++i)
			if(isalpha(c[i]))
			{
				if(j==0)
					str[n] = new char [200];
				str[n][j++] = c[i];
			}
			else if(j)
			{
				str[n++][j++]='\0';
				j=0;
			}
		if(j)
			str[n++][j++]='\0';
		// hash 
		hash = WordHash(c);
		for(int i=0;i<n;++i)
			h[i] = tmp_words[i];
	}
};

void makeED(Sepword &s,int ed)
{
	if(ed==2)
	{
		s.put_to_answer();
//		s.print();
		return ;
	}
	//substitude
	int tmp = 9-ed;// don't use same memory
	for(int i=s.n-1;i>=0;--i)
	{
		s.swap(i,tmp);
		for(int j=0;j<questionN;++j)
		{
			s.candi_in(i,j);
			s.hash = Gohash(s.h,s.n);// can be improved
//			s.print();
			makeED(s,ed+1);
		}
		s.swap(i,tmp);
	}

	//add
	++s.n;
	for(int i=s.n-1;i>=0;--i)
	{
		s.swap(i,i+1);
		for(int j=0;j<questionN;++j)
		{
			s.candi_in(i,j);
			s.hash = Gohash(s.h,s.n);// can be improved
//			s.print();
			makeED(s,ed+1);
		}
	}
	for(int i=0;i<s.n;++i)
		s.swap(i,i+1);
	--s.n;

	//delete
	if(s.n==0)
		return ;

	--s.n;
	for(int i=s.n;i>=0;--i)
	{
		s.swap(i,tmp);
		s.hash = Gohash(s.h,s.n);// can be improved
//		s.print();
		makeED(s,ed+1);
	}
	for(int i=0;i<=s.n;++i)
		s.swap(i,tmp);
	++s.n;
//	s.print();
}

void init_candihash()
{
	for(int i=0;i<questionN;++i)
	{
		ll sum=0;
		for(int j=0;candi[i][j];++j)
			sum = (sum*Word_mult+candi[i][j])%Word_mod;
		hash_cadi[i] = (int)sum;
	}
}

int main()
{
	Filetohashtable();
//	puts("ok");

	init_candihash();

	char c[1000];
	while( fgets(c,1000,stdin) )
	{
		printf("query: %s",c);
		Sepword s;
		s.input_sep(c);
		ans.clear();
		makeED(s,0);
		// answer
		std::sort(ans.begin(),ans.end(),wordcomp);
		if(ans.size()==0)
		{
			puts("NONE");
			continue;
		}
		int n=1;
		for(int i=1;i<ans.size();++i)
			if( ans[i-1] != ans[i])
				ans[n++] = ans[i];
		printf("output: %d\n",n);
		for(int i=0;i<n;++i)
			printf("%s\t%d\n",ans[i]->str,ans[i]->feq);
	}
	return 0;
}


