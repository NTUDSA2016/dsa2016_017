#include<bits/stdc++.h>

#define wordLen 40
#define ll long long 
using std::string;

struct link
{
	int num;
	link *next;
}*nextp,*hashTable[100000000];

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

struct word
{
	char str[220];
	int feq;
}ori_data[5774000];

//not good . put it outside

int tmp_words[10];
int tmp_wh;
int WordHash(char *c)
{
	int sum=0,len=0,start=-1,end=0;
	
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
			end = i ;
		}

	}
	tmp_words[len++] = -1;
	tmp_wh = Gohash(tmp_words);
	return end;
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
		int i = WordHash(c);

		// modify the string
		c[i]='\0';
		int dig=0;
		for(;!isdigit(c[i]);++i);
		for(;isdigit(c[i]);++i)
			dig = dig*10 + c[i]-'0';
		ori_data[len].feq = dig;

		// put into hashtable
//		hashTable[tmp_wh]++;
		nextp->num = len;
		nextp->next = hashTable[tmp_wh];
		hashTable[tmp_wh] = nextp++ ;
		++len;
	}
	fclose(f);
}

int main()
{
	Filetohashtable();
	std::map<int,int> map;
//	for(int i=0;i<100000000;++i)
//		if(hashTable[i]>0)
//			map[hashTable[i]]++;
//	for(auto i:map)
//		printf("%d %d\n",i.first,i.second);
	puts("ok");
	return 0;
}


