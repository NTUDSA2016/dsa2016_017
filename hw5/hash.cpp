#include<bits/stdc++.h>

#define wordLen 40
#define ll long long 
using std::string;

char *hashTable[100000000];

ll Gohash(char *c)
{
	ll sum=0;
	for(int i=0;c[i];++i)
		sum = (sum*2328211+c[i])%99999989;
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


int main()
{
	Filetohashtable("dict");
	puts("ok");

	return 0;
}
	

