#include<bits/stdc++.h>
#define ll long long 
#define wordLen 40
using std::string;

char *hashTable[100000000];

#define Hash_mult 2328211
#define Hash_mod 99999989

ll Gohash(char *c)
{
	ll sum=0;
	for(int i=0;c[i];++i)
		sum = (sum*Hash_mult+c[i])%Hash_mod;
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
//	printf("%d\n",max);
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

char v_candi[50000][wordLen];
int hash_candi[50000];
int candi_size;

void makeED(char *c)
{
	int len = strlen(c);
	ll h; 
	ll hm=1;
	char tmp;
//	candi_size=0;

	// substitude
	hm=1;
	h = Gohash(c);
	for(int i=len-1;i>=0;--i)
	{
		tmp = c[i];
		h = ((h-(ll)hm*tmp)%Hash_mod+Hash_mod)%Hash_mod;
		for(char j='a';j<='z';++j)
			if(tmp !=j)
			{
				c[i]=j;
				hash_candi[candi_size] = (h+(ll)j*hm)%Hash_mod;
				strcpy(v_candi[candi_size++],c);
			}
		c[i] = tmp;
		h = (h+(ll)hm*tmp)%Hash_mod;
		hm = hm * Hash_mult % Hash_mod;
	}

	// delete
	tmp = c[len];
	for(int i=len-1;i>=0;--i)
	{
		std::swap(c[i],tmp);
		hash_candi[candi_size] = Gohash(c);
		strcpy(v_candi[candi_size++],c);
	}
	for(int i=len-1;i>=0;--i)
		c[i+1]=c[i];
	c[0]=tmp;

	//add
	hm=1;
	for(int i=len;i>=0;--i)
	{
		c[i+1] = c[i];
		c[i]=1;
		h = Gohash(c);
		for(char j='a';j<='z';++j)
		{
			c[i]=j;
			hash_candi[candi_size] = (h+(ll)(j-1)*hm)%Hash_mod;
			strcpy(v_candi[candi_size++],c);
		}
		hm = hm * Hash_mult % Hash_mod;
	}
	for(int i=0;i<=len;++i)
		c[i] = c[i+1];

	//transpose
	h = Gohash(c);
	hm=1;
	for(int i=len-1;i>0;--i)
	{
		hash_candi[candi_size] = ( ( h + 
				(ll)(c[i]-c[i-1])*hm%Hash_mod*Hash_mult%Hash_mod + 
				(ll)(c[i-1]-c[i])*hm%Hash_mod) %Hash_mod + Hash_mod)%Hash_mod;
		std::swap(c[i-1],c[i]);
		strcpy(v_candi[candi_size++],c);
		std::swap(c[i-1],c[i]);
		hm = hm * Hash_mult % Hash_mod;
	}

}

char* getfromhash(char *c,int h)
{
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

	//correct
	if(getfromhash(c,Gohash(c)))
	{
		puts(" OK");
		return ;
	}

	// too long will have bug
	// the max length of word in dict is 34
	if( strlen(c) >36)
	{
		puts(" NONE");
		return;
	}

	//ED1
	std::vector<char *> v;
	candi_size = 0;
	makeED(c);
	int tmpsize = candi_size;
	for(int i=0;i<tmpsize;++i)
	{
		char *ans = getfromhash( v_candi[i] , hash_candi[i]);
		if(ans)
			v.push_back(ans);
	}

	//ED2
	for(int i=0;i<tmpsize;++i)
	{
		candi_size = tmpsize; // conserve memory
		makeED(v_candi[i]);
		for(int j=tmpsize;j<candi_size;++j)
		{
			char *ans = getfromhash( v_candi[j] , hash_candi[j]);
			if(ans)
				v.push_back(ans);
		}
	}


	// output
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
	Filetohashtable("/tmp2/dsa2016_hw5/cmudict-0.7b");
	char c[1000];
	while( fgets(c,1000,stdin) )
	{
		for(int i=0;c[i];++i)
			if(c[i]==' ' || c[i]=='\t' || c[i]=='\n')
			{
				c[i]='\0';
				break;
			}
		find(c);
	}
	return 0;
}

