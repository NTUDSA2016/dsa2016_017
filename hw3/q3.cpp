#include<bits/stdc++.h>

#define MM 120
#define VI std::vector<int>
using std::string;

int *st_arr ,st_wh[MM],st_m;
std::vector< std::pair<int,int> > stat[2];
VI st_lim;

void to_statical();
bool canput();
void must(int);
void smart_solve();
bool iscorrect();

struct nono
{
	int map[MM][MM];
	int n,m;
	bool en[MM];
	VI row[MM];

	void init(int &,int &);
	void limit_input();
	void copyout(int&);
	bool copymap(nono&); 
	void OUTPUT();
	bool check();
	bool isdone();


	void law();

}gram[2];

void nono::init(int &_n,int &_m)
{
	n = _n ; m = _m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			map[i][j] = 0 ;
	for(int i=0;i<n;++i)
		en[i]=true;
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

bool nono::copymap(nono &o)
{
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			if( map[i][j] != o.map[j][i])
			{
				map[i][j] = o.map[j][i] ;
				en[i]=true;
			}
	for(int i=0;i<n;++i)
		if( en[i] )
			return 1;
	return 0;//no change
}

void nono::copyout(int &i)
{
	st_arr = map[i];
	st_lim = row[i];// copy no very good
	st_m   = m ;
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

bool nono::check()
{
	for(int i=0;i<n;++i)
	{
		copyout(i);
		to_statical();
		if(st_lim.size() != stat[1].size() )
			return 0;
		for(int i=0;i<st_lim.size();++i)
			if ( st_lim[i] != stat[1][i].second - stat[1][i].first)
				return 0;
	}
	return 1;
}

bool nono::isdone()
{
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			if(map[i][j] == 0)
				return 0;
	return 1;
}

void nono::law()
{
	for(int i=0;i<n;++i)
		if(en[i])
		{
			copyout(i);
			must(1);
			must(-1);
			en[i]=false;
		}
}

void to_statical()
{
	stat[0].clear();stat[1].clear();// 0 1 no -1
	int wh = 0,
		bef=-1;
	for(int i=0;i<st_m;++i)
	{
		int &now = st_arr[i];
		if( now==-1 && bef!=-1)
			++wh;
		else if( bef < now)
			for(int j=bef+1;j<=now;++j)
				stat[ j ].push_back( std::make_pair(i,i) );

		for(int j=0;j<=now;++j)
			stat[j].back().second=i;
		bef = now;
		st_wh[i] = wh ;
	}

	for(auto &i:stat[0])
		++i.second;
	for(auto &i:stat[1])
		++i.second;
}

bool putleft(int l,int& now)
{
	if(now >= stat[0].back().second)
		return 0;// no way to placed
	int v0 = st_wh[now]; // v0 must < stat[0].size()
	if( now < stat[0][v0].first )
		now = stat[0][v0].first ;
	while(stat[0][v0].second - now < l )
	{
		++v0;
		if(v0 == stat[0].size() )
			return 0;//end 
		now = stat[0][v0].first;
	}
	now += l+1 ;
	return 1;
}

#define MAXX(a,b,c) std::max(std::max(a,b),c)
bool dfs_pair(int v1,int l1,int now)
{
	//pair is over
	if(v1==stat[1].size())
	{
		// make sure all the limit can be placed
		for(l1;l1<st_lim.size();++l1)
			if ( !putleft(st_lim[l1],now) )
				return 0;
		return 1;
	}

	//start 
	int v0 = st_wh[ stat[1][v1].first ];
	std::pair<int,int> &v = stat[0][v0];
	for(l1;l1<st_lim.size();++l1)
	{
		//start pair
		int &lim = st_lim[l1];
		for(int i=v1;i<stat[1].size() && 
				st_wh[ stat[1][i].first ]==v0 &&  // a group
				stat[1][i].second - stat[1][v1].first <= lim ;// small than limit
				++i)
		{
			int here = MAXX(now,v.first,stat[1][i].second-lim);
			if(here+lim>v.second)  // to big 
				break;
			if( i+1 < stat[1].size() && here+lim >= stat[1][i+1].first )
				continue; //cover another 
			if( dfs_pair(i+1,l1+1,here+lim+1) ) // recursive
				return 1;
		}

		// not pair
		if(!putleft(lim,now))   
			return 0;
		if( now > stat[1][v1].first ) // not touch to v1
			return 0; 
	}
	
	return 0;
}
bool canput()
{
	return  dfs_pair(0,0,0) ;
}

void must(int it)
{
	for(int i=0;i<st_m;++i)
		if( st_arr[i] == 0)
		{
			st_arr[i] = -it;
			to_statical();
			if( !canput() )
				st_arr[i] = it;
			else
				st_arr[i] = 0;
		}
}

		

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

bool iscorrect()
{
	if( !gram[0].check() )
		return 0;
	gram[1].copymap(gram[0]);
	if( !gram[1].check() )
		return 0;
	return 1;
}

void test()
{
	exit(0);
}

void smart_solve()
{
	int now=1;
	do
	{
		now = !now;
		gram[now].law();
//		gram[now].OUTPUT();
//		puts("");
	}while( gram[!now].copymap( gram[now] ) );
	// gram 0 1  is the same
}

int dfs_n,dfs_m;
bool dfs_nono(int x,int y)
{
	nono &g = gram[0];
	int has=0;
	int map[ dfs_n ][ dfs_m ];
	while(1)
	{
		// find the 0 in the map
		while(y>=dfs_m || x>=dfs_n || g.map[x][y]!=0)
		{
			++y;
			if(y>=dfs_m)
				++x,y=0;
			if(x>=dfs_n)
			{
				if(has==0)
					return 1;
				else
					return 0;
			}
		}
		++has; // find one
		// test is ok to put -1;
		g.map[x][y] = -1 ;
		g.copyout(x);
		to_statical();
		if(canput())
		{
			// copy one n*m
			for(int i=0;i<dfs_n;++i) 
				for(int j=0;j<dfs_m;++j)
					map[i][j] = g.map[i][j];
		
			// why not just enable one , because when retraveal here the en is bad
			for(int i=0;i<dfs_n;++i)
				gram[0].en[i]=1;
			for(int i=0;i<dfs_m;++i)
				gram[1].en[i]=1;
			smart_solve();
			if( gram[0].isdone())
				if( iscorrect() )
					return 1;
				else
					return 0;
			else if( dfs_nono(x,y+1) )
				return 1;// correct
			
			// not correct restore the map
			for(int i=0;i<dfs_n;++i) // 
				for(int j=0;j<dfs_m;++j)
					g.map[i][j] = map[i][j] ;
		}
		g.map[x][y++]=0;
	}
}


bool solve()
{
	smart_solve();
	dfs_n = gram[0].n;
	dfs_m = gram[0].m;
	if( !gram[0].isdone() )
		if( !dfs_nono(0,0) )
			return 0;
	return iscorrect();
}

int main()
{
//	test();
	INPUT();
	if ( !solve() )
		puts("error");
	gram[0].OUTPUT();

};
