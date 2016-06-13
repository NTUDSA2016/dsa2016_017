#include<bits/stdc++.h>

#define MaxData 50000000
#define wordLen 40
#define ll long long 
#define VS std::vector<std::string>

VS prepos = {"of", "to", "in", "for", "with", "on", "at","by", "from", "up", "about", "than", "after", "before", "down", "between", "under", "since", "without", "near"};
std::set<std::string> prepos_map;

char **ori_data;
int    ori_n=0;

void Filetohashtable()
{
	// hashtable will be initized with 0
	ori_n = 0;
	ori_data = new char* [MaxData];
	for(int i=0;i<MaxData;++i)
		ori_data[i] = new char [220]; 

	char c[1000];
	for(int i=2;i<=5;++i)
	{
		sprintf(c,"/tmp2/dsa2016_project/%dgm.small.txt",i);
		FILE *f = fopen(c,"r");
		while( fgets(ori_data[ori_n],220,f)) 
			ori_n++;
		fclose(f);
	}
}

int myrand(int mod)
{
	return (int)( ((ll)rand()*rand()+rand())%mod);
}


void print(VS &v)
{
	if(!v.size())
		return ;//no size
	int st=0;
	for(auto &i:v)
	{
		if(st++)
			putchar(' ');
		std::cout<< i;
	}
	puts("");
}


VS charTovector(char *c)
{
	VS v;
	std::stringstream ss(c);
	std::string s;
	while( ss >> s)
		v.push_back(s);
	return v;
}

void add(VS &v)
{
	v.insert(v.begin()+myrand(v.size()+1),prepos[myrand(prepos.size())]);
}
void del(VS &v)
{
	std::vector<int> pv;
	for(int i=0;i<v.size();++i)
		if( prepos_map.count(v[i]) )
			pv.push_back(i);
	if( pv.size() )
		v.erase(v.begin()+pv[myrand(pv.size())]);
}
void sub(VS &v)
{
	std::vector<int> pv;
	for(int i=0;i<v.size();++i)
		if( prepos_map.count(v[i]) )
			pv.push_back(i);
	if( pv.size() )
		v[pv[myrand(pv.size())]] = prepos[myrand(prepos.size())];
}

int main()
{
	prepos_map.clear();
	for(auto &i:prepos)
		prepos_map.insert(i);

/*
	char c[1000];
	while( fgets(c,1000,stdin) )
	{
		VS v = charTovector(c);
		print(v);
		sub(v);
		print(v);
	}
	return 0;
*/

	Filetohashtable();
	srand(time(NULL));
	for(int i=0;i<10000;++i)
	{
		VS v = charTovector(ori_data[ myrand(ori_n) ]);
		v.resize(v.size()-1);// no digit
		int n = myrand(7);
		while(n--)
			switch( myrand(3) )
			{
				case 0:add(v);break;
				case 1:del(v);break;
				case 2:sub(v);break;
			}
		print(v);
	}
	return 0;
}


