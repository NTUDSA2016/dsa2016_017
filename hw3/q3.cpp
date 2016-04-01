#include<bits/stdc++.h>

#define MM 120
#define VI std::vector<int>
using std::string;

struct nono
{
	int map[MM][MM];
	int n,m;
	VI row[MM];

	void init(int &,int &);
	void limit_input();
	void OUTPUT();

#define VII std::vector< std::pair<int,int> >

	void law();
	void to_statical(int*, VII*);
	bool canput(VII*, VI&);
	void mustare(int*, VII*, VI&);
	void mustnot(int*, VII*, VI&);

}gram[2];

void nono::init(int &_n,int &_m)
{
	n = _n ; m = _m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			map[i][j] = 0 ;
}

void nono::limit_input()
{
	for(int i=0;i<n;++i)
	{
		row[i].clear();
		std::string s;
		getline(std::cin,s);
		std::istringstream iss(s);
		int t;
		while( iss >> t)
			row[i].push_back(t);
	}
}

void nono::OUTPUT()
{
	for(int i=0;i<n;++i,puts(""))
		for(int j=0;j<m;++j)
			switch(map[i][j])
			{
				case -1:putchar('.');break;
				case  0:putchar('?');break;
				case  1:putchar('#');break;
			}
}

void nono::law()
{
	VII v[2];// 0 -> can put  1-> is put , [first,second) 
	for(int i=0;i<n;++i)
	{
		int *arr = map[i];
		to_statical(arr,v);
//			mustare(arr,v,row[i]);
//			mustnot(arr,v,row[i]);
	}
}

void nono::to_statical(int *arr, VII *v)
{
	v[0].clear();v[1].clear();// 0 1 no -1
	int bef=-1;
	for(int i=0;i<m;++i)
	{
		if( bef < arr[i])
		{
			v[ arr[i] ].push_back( std::make_pair(i,i) );
			if(bef == -arr[i] )
				v[0].push_back( std::make_pair(i,i) );
		}

		for(int j=0;j<=arr[i];++j)
			v[j].back().second=i;
		bef = arr[i];
	}

	for(auto &i:v[0])
		++i.second;
	for(auto &i:v[1])
		++i.second;
}

bool nono::dfs_pair(VII *v,VI &lim,int v0,int v1,int l1,int now)
{
	//pair is over
	if(v1==v[1].size())
	{
		// make sure all the limit can be placed
		for(l1;l1<lim.size();++l1)
		{
			if(v0 == v[0].size() )
				return 0;
			while(v[0][v0].second - now < lim[l1] )
			{
				++v0;
				if(v0 == v[0].size() )
					return 0;
				now = v[0][v0].first;
			}
			now += lim[i]+1 ;
		}
		return 1;
	}

	//start 
	for(l1;l1<lim.size();++l1)
	{
		//start pair
		if( lim[l1] >= v[1][v1].second - v[1][v1].first)
		{
//			int s = std::max( v[1][v1].second - lim[l1] , v[0][v0]);
		}
		else // not pair
		{
		}
	}
	
		
}
		// be sure the all of the right of l1 can be placed

bool nono::canput(VII *v,VI &lim)
{
	return  dfs_pair(v,lim,0,0,0,0) ;
}

	
		

//	void mustare(int* arr, VII *v)
//	{
		

void INPUT()
{
	int n,m;
	scanf("%d%d",&n,&m);
	std::cin.ignore(INT_MAX,'\n');//first newline

	gram[0].init(n,m);
	gram[1].init(m,n);
	gram[0].limit_input();
	gram[1].limit_input();
}

void test()
{
	VII v[2];int c[1000],n;
	scanf("%d",&n);
	gram[0].m = n;
	for(int i=0;i<n;++i)scanf("%d",c+i);
	gram[0].to_statical(c,v);
	for( auto i:v[0])
		printf("%d %d\n",i.first,i.second);
	puts("");
	for( auto i:v[1])
		printf("%d %d\n",i.first,i.second);
	exit(0);
}


int main()
{
//	test();
	INPUT();
	gram[0].law();
	gram[0].OUTPUT();
};
