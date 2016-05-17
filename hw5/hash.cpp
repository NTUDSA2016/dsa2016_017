#include<bits/stdc++.h>

#define ll long long 

std::vector<std::string> hashTable[100000000];
ll Gohash(char *c)
{
	ll sum=0;
	for(int i=0;c[i];++i)
		sum = (sum*2328211+c[i])%99999989;
	return sum;
}

void Filetohashtable()
{
	FILE *f = fopen("dict","r");
	int len=0;
	char c[1000];
	while( fgets(c,1000,f) )
	{
		int alp = 1,len=0;
		for(len=0;c[len]!=' ';++len)
			if( !isalpha(c[len]) && c[len]!='-')
			{
				alp=0;
				break;
			}
		if(alp)
		{
			c[len]='\0';
			/*
			for(int i=0;i<len;++i)
				printf("%c",c[i]);
			puts("");
			*/
			hashTable[gohash(c)].pb(c);
		}
	}
	fclose(f);
}

int main()
{

	return 0;
}
	

