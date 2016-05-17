#include<bits/stdc++.h>
#define ll long long 
#define wordLen 36
using std::string;

char *hashTable[100000000];

ll Gohash(char *c)
{
	ll sum=0;
	for(int i=0;c[i];++i)
		sum = (sum*2328211+c[i])%99999989;
	return sum;
}

void Filetohashtable(std::string dir="dict")
{
	// hashtable will be initized with 0
	FILE *f = fopen(dir.c_str(),"r");
//	FILE *fout = fopen("test","w");
	int len=0,max=0;
	char c[1000];
	while( fgets(c,1000,f) )
	{
		int len=0;
		for(len=0;c[len]!=' ';++len)
			if('A'<=c[len] && c[len]<='Z' )
				c[len]=c[len]-'A'+'a';
		if(c[1]!=';')
		{
			c[len]='\0';
//			fprintf(fout,"%s\n",c);
			int h = Gohash(c);
			if ( hashTable[h])
			{
				if( hashTable[h+5])
					puts("no");
				hashTable[h+5] = new char[wordLen] ;
				strcpy(hashTable[h+5],c);
			}
			else
			{
				hashTable[h] = new char[wordLen] ;
				strcpy(hashTable[h],c);
			}
			if(len>max)
				max=len;
		}
	}
	printf("%d\n",max);
	fclose(f);
//	fclose(fout);
}

// DP
int DP_ED[wordLen][wordLen];
int EditDistance(char *ca,char *cb)
{
	int Lena = strlen(ca),
		Lenb = strlen(cb);
	for(int j=0;j<=Lena;++j)
		DP_ED[0][j] = j ;	


	for(int i=1;i<=Lenb;++i)
	{
		DP_ED[i][0] = i;
		for(int j=1;j<=Lena;++j)
		{
			// add remove
			DP_ED[i][j] = std::min( DP_ED[i-1][j], DP_ED[i][j-1]) + 1;
			// same or replace
			DP_ED[i][j] = std::min( DP_ED[i  ][j  ], 
									DP_ED[i-1][j-1]+ (cb[i-1] != ca[j-1]) );
			// transport
			if( i>=2)// add 
				for( int k=j-1;k>=1 && DP_ED[i][k+1] == DP_ED[i][k]+1;--k)
					if( cb[i-1] == ca[k-1] && cb[i-2] == ca[j-1]  )
						DP_ED[i][j] = std::min( DP_ED[i][j], 
										DP_ED[i-2][k-1] + j-k );
			if( j>=2)// remove
				for( int k=i-1;k>=1 && DP_ED[k+1][j] == DP_ED[k][j]+1;--k)
					if( cb[k-1] == ca[j-1] && cb[i-1] == ca[j-2]  )
						DP_ED[i][j] = std::min( DP_ED[i][j], 
										DP_ED[k-1][j-2] + i-k );
		}
	}

/*debug
	for(int i=0;i<=Lenb;++i,puts(""))
		for(int j=0;j<=Lena;++j)
			printf("%2d ",DP_ED[i][j]);
*/
	return DP_ED[Lenb][Lena];
}

char v_candi[1000000][wordLen];
int candi_size;

void makeED(char *c)
{
	int len = strlen(c);
	char tmp;
//	candi_size=0;

	// substitude
	for(int i=0;i<len;++i)
	{
		tmp = c[i];
		for(char j='a';j<='z';++j)
			if(tmp !=j)
			{
				c[i]=j;
				strcpy(v_candi[candi_size++],c);
			}
		c[i] = tmp;
	}

	// delete
	tmp = c[len];
	for(int i=len-1;i>=0;--i)
	{
		std::swap(c[i],tmp);
		strcpy(v_candi[candi_size++],c);
	}
	for(int i=len-1;i>=0;--i)
		c[i+1]=c[i];
	c[0]=tmp;

	//add
	for(int i=len;i>=0;--i)
	{
		c[i+1] = c[i];
		for(char j='a';j<='z';++j)
		{
			c[i]=j;
			strcpy(v_candi[candi_size++],c);
		}
	}
	for(int i=0;i<=len;++i)
		c[i] = c[i+1];

	//transpose
	for(int i=1;i<len;++i)
	{
		std::swap(c[i-1],c[i]);
		strcpy(v_candi[candi_size++],c);
		std::swap(c[i-1],c[i]);
	}

}

char* getfromhash(char *c)
{
	int h = Gohash(c);
	if( hashTable[h] == NULL)
		return NULL;
	else if( strcmp(hashTable[h],c)==0 )
		return hashTable[h];
	else if( hashTable[h+5]!=NULL && strcmp(hashTable[h+5],c)==0 )
		return hashTable[h+5];
	else
		return NULL;
}

bool mycomp(char *a,char *b)
{
	return strcmp(a,b) <0;
}

void find(char *c)
{
	printf("%s ==>",c);
	char *ans;

	//correct
	ans = getfromhash(c);
	if(ans)
	{
		puts(" OK");
		return ;
	}

	//ED1
	std::vector<char *> v;
	candi_size = 0;
	makeED(c);

	//ED2
	int tmpsize = candi_size;
	for(int i=0;i<tmpsize;++i)
		makeED(v_candi[i]);
	
	for(int i=0;i<candi_size;++i)
	{
		char *ans = getfromhash( v_candi[i] );
		if(ans)
			v.push_back(ans);
	}

	if(v.size())
	{
		std::sort(v.begin(),v.end(),mycomp);
		printf(" %s",v[0]);
		for(int i=1;i<v.size();++i)
			if( strcmp(v[i-1],v[i]) )
				printf(" %s",v[i]);
		puts("");
	}
	else
		puts(" NONE");
}

int main()
{
	Filetohashtable();
	char ca[wordLen];
	while(std::cin>> ca )
	{
		find(ca);
	}
	return 0;
}

