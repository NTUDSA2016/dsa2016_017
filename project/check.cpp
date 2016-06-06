#include<bits/stdc++.h>

#define wordLen 40
#define ll long long 
using std::string;

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
		{
			if(i)putchar(' ');
			printf("%s",str[i]);
		}
		printf("\n",hash);
	}

	inline void swap(int a,int b)
	{
		std::swap(str[a],str[b]);
		std::swap(  h[a],  h[b]);
	}
	inline void candi_in(int i,int c)
	{
		str[i] = 	 candi[c];
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
		return c[k]=='\0';
	}

	Sepword(const Sepword &s) 
	{
		n = s.n;
		for(int i=0;i<n;++i)
		{
			str[i] = new char [220];
			strcpy(str[i],s.str[i]);
		}
		hash = s.hash;
	}
	void operator = (const Sepword &s) 
	{
		n = s.n;
		for(int i=0;i<n;++i)
		{
			str[i] = new char [220];
			strcpy(str[i],s.str[i]);
		}
		hash = s.hash;
	}

	void input_sep(char *c)
	{
		int j=0;
		n=0;
		hash=0;
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
			else if( isdigit(c[i]) )
				hash = hash*10 + c[i] - '0' ;
	}
}*ori_data;
int ori_n;

void Filetohashtable()
{
	// hashtable will be initized with 0
	ori_n = 0;
	ori_data = new Sepword [50000000];
	char c[1000];
	for(int i=2;i<=5;++i)
	{
		sprintf(c,"/tmp2/dsa2016_project/%dgm.small.txt",i);
		FILE *f = fopen(c,"r");
		char c[220];
		while( fgets(c,220,f) )
		{
			ori_data[ori_n++].input_sep(c);
		}
		fclose(f);
	}
}


std::vector<Sepword> ans;

char the[100]={"XXXXGAY"};
char hey[100]={"BOYO"};

void makeED(Sepword &s,int ed)
{
	if(s.n==0)
		return ;
	if(ed==2)
	{
		ans.push_back(s);
		return ;
	}
	int tmp = 9-ed;// don't use same memory

	//delete
	--s.n;
	for(int i=s.n;i>=0;--i)
	{
		s.swap(i,tmp);
//		s.print();
		makeED(s,ed+1);
	}
	for(int i=0;i<=s.n;++i)
		s.swap(i,tmp);
	++s.n;

	//substitude
	for(int i=s.n-1;i>=0;--i)
	{
		s.swap(i,tmp);
		s.str[i] =  the;
		makeED(s,ed+1);
		s.str[i] =  hey;
		makeED(s,ed+1);
		s.swap(i,tmp);
	}

	//add
	++s.n;
	for(int i=s.n-1;i>=0;--i)
	{
		s.swap(i,i+1);
		s.str[i] = the;
		makeED(s,ed+1);
		s.str[i] =  hey;
		makeED(s,ed+1);
	}
	for(int i=0;i<s.n;++i)
		s.swap(i,i+1);
	--s.n;

//	s.print();
}

int myrand(int mod)
{
	return (int)( ((ll)rand()*rand()+rand())%mod);
}

int main()
{
	Filetohashtable();
//	puts("ok");

	srand(time(NULL));
	char c[1000];
//	while( fgets(c,1000,stdin) )
	for(int i=0;i<10000;++i)
	{
//		printf("query: %s",c);
//		Sepword s;
//		s.input_sep(c);
		ans.clear();
		makeED(ori_data[myrand(ori_n)],0);
		// answer
		for(int j=0;j<5 && j<ans.size();++j)
			ans[myrand(ans.size())].print();
//		for(auto &i:ans)
//			i.print();
	}
	return 0;
}


