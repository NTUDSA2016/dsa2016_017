
#include<bits/stdc++.h>

#define WordLen 40
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
int ori_n;

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

int main()
{
	fileTohashtable();
	return 0;
}

