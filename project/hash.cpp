#include<bits/stdc++.h>

#define wordLen 40
#define ll long long 
using std::string;

int hashTable[100000000];

#define Hash_mult 2350001
#define Hash_mod 99999989

int Gohash(int *c)
{
	ll sum=0;
	for(int i=0;c[i]!=-1;++i)
		sum = (sum*Hash_mult+c[i])%Hash_mod;
	return (int)sum;
}

#define Word_mult 11261
#define Word_mod 46301
// should not bigger than 46340

void WordHash(char *c)
{
	int words[10],sum=0,len=0,start=-1,fre=0,hashall=0;

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
			words[len++] = sum;
			hashall += sum;
			sum = 0;
			start=-1;
		}
		else if( isdigit(c[i]))
		{
			int j;
			for(j=i-1;!isalpha(c[j-1]);--j);
			c[j] = '\0';
			for(;isdigit(c[i]);++i)
				fre = fre*10 + c[i]-'0';
//			printf("%d\n",fre);
			break;
		}

	}
	words[len++] = -1;
	ll  h = Gohash(words);
	if( !hashTable[h])
		 hashTable[h]++;
	else 
		  hashTable[ (h+hashall)%Hash_mod]++;
}

int main()
{
	int max=0,len=0;
	char c[1000];
	FILE *f = fopen("5gm.txt","r");
	while( fgets(c,1000,f) )
	{
		if(strlen(c)>max)
			max = strlen(c);
		++len;
		WordHash(c);
	}
	printf("%d %d\n",max,len);
	std::map<int,int> map;
	for(int i=0;i<100000000;++i)
		if(hashTable[i]>0)
			map[hashTable[i]]++;
	for(auto i:map)
		printf("%d %d\n",i.first,i.second);
	return 0;
}


