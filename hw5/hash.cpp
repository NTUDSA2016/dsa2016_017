#include<bits/stdc++.h>

#define wordLen 40
#define ll long long 
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

void Filetohashtable(string dir="dict")
{
	// hashtable will be initized with 0
	FILE *f = fopen(dir.c_str(),"r");
//	FILE *fout = fopen("test","w");
	int len=0,max=0;
	char c[1000];
	while( fgets(c,1000,f) )
	{
		// be careful for someword like dÉjÀ which may have minus hash num
		int alp = 1,len=0;
		for(len=0;c[len]!=' ';++len)
			if('A'<=c[len] && c[len]<='Z' )
				c[len]=c[len]-'A'+'a';
		if(c[1]!=';')
		{
			c[len]='\0';
//			fprintf(fout,"%s\n",c);
			/*
			for(int i=0;i<len;++i)
				printf("%c",c[i]);
			puts("");
			*/
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

char v_candi[50000][wordLen];
int hash_candi[50000];
int candi_size;

void makeED(char *c)
{
	int len = strlen(c);
	ll h = Gohash(c);
	ll hm=1;
	char tmp;
//	candi_size=0;

	// substitude
	hm=1;
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


int main()
{
//	Filetohashtable("dict");
	
	char c[1999];
	while( std::cin >> c)
	{
		candi_size=0;
		makeED(c);
		int ok=1;
		for(int i=0;i<candi_size;++i)
		{
			printf("%s %d %lld\n",v_candi[i],Gohash(v_candi[i]),hash_candi[i]);
			if( Gohash(v_candi[i]) != hash_candi[i] )
				ok=0;
		}
		if(ok)
			puts("ok");
		else
			puts("no");
	}
	return 0;
}
	

