#include<bits/stdc++.h>

#define MM 30
#define VI std::vector<int>
using std::string;

char *st_arr ;
int st_wh[MM],st_m;
VI *st_lim;//use pointer
std::vector< std::pair<int,int> > stat[2];

void INPUT();
void to_statical();
bool solve();
bool dfs_nono(int,int);
void smart_solve();
void must(int);
bool canput();
bool dfs_pair(int, int, int);
bool putleft(int,int&);
bool iscorrect();

struct nono
{
	char map[MM][MM];
	int n,m;
	bool en[MM];
	VI row[MM];

	void init(int &,int &);
	void limit_input();
	void OUTPUT();
	void copyout(int&);
	bool copymap(nono&); 
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
	st_lim = row+i ;// copy no very good
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
		if(st_lim->size() != stat[1].size() )
			return 0;
		for(int i=0;i<st_lim->size();++i)
			if ( (*st_lim)[i] != stat[1][i].second - stat[1][i].first)
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
		char &now = st_arr[i];
		if( now==-1 && bef!=-1)
			++wh;
		else if( bef < now)
		{
			if(now==1)
				stat[ 1 ].push_back( std::make_pair(i,i) );
			if(bef==-1)
				stat[ 0 ].push_back( std::make_pair(i,i) );
		}

		if( now >= 0 )
		{
			if(now==1)
				stat[1].back().second=i;
			stat[0].back().second=i;
		}
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
bool dfs_pair1(int v1,int l1,int now)
{
	//pair is over
	if(v1==stat[1].size())
	{
		// make sure all the limit can be placed
		for(l1;l1<st_lim->size();++l1)
			if ( !putleft((*st_lim)[l1],now) )
				return 0;
		return 1;
	}
	if(l1==st_lim->size())// no one can pair
		return 0;
	
	//init
	int v0 = st_wh[ stat[1][v1].first ];
	std::pair<int,int> &v = stat[0][v0];

	// put to left as possible
	std::stack<int> s;
	for(l1;l1<st_lim->size();++l1)
	 {
		s.push( now ) ;
		if(!putleft((*st_lim)[l1],now))   
			break;
		if( now > stat[1][v1].first ) // not touch to v1
			break;
	 }
	if(l1 == st_lim->size())
		--l1;// no pop because it end 
	
	// start recursive
	while( s.size() )
	{
		now = s.top();
		//start pair
		int &lim = (*st_lim)[l1];
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
			if( dfs_pair1(i+1,l1+1,here+lim+1) ) // recursive
				return 1;
		}
		s.pop();
		--l1;
	}
	return 0;
}

bool dfs_pair(int v1,int l1,int now)
{
	//pair is over
	if(v1==stat[1].size())
	{
		// make sure all the limit can be placed
		for(l1;l1<st_lim->size();++l1)
			if ( !putleft((*st_lim)[l1],now) )
				return 0;
		return 1;
	}

	//start 
	int v0 = st_wh[ stat[1][v1].first ];
	std::pair<int,int> &v = stat[0][v0];
	for(l1;l1<st_lim->size();++l1)
	{
		//start pair
		int &lim = (*st_lim)[l1];
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

	stat[0].reserve(MM);// don't change size
	stat[1].reserve(MM);
	gram[0].init(n,m);
	gram[1].init(m,n);
	gram[0].limit_input();
	gram[1].limit_input();
}

bool iscorrect()
{
	if( !gram[0].check() )
		return 0;
//	gram[1].copymap(gram[0]); // not must because we do smart solve before it
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


nono stack_no[MM*MM];int stack_size;

bool dfs_nono(int x,int y)
{
//	static int ti=0;
//	printf("%d\n",ti++);
	nono &g = gram[0];
	nono &no = stack_no[stack_size++];// this is quicker
	while(1)
	{
		// find the 0 in the map
		while(y>=g.m || x>=g.n || g.map[x][y]!=0)
		{
			++y;
			if(y>=g.m)
				++x,y=0;
			if(x>=g.n)
				{ --stack_size; return 0;}// because if it is not find 0 it is correct 
		}
		// test is ok to put 1;
		g.en[x]=1;
		g.map[x][y] = 1 ;
		no = g;// copy one
	
		// it will always canput while smart solve is correct
		smart_solve();
		if( g.isdone())
		{
			if( iscorrect() )
				{--stack_size;return 1;}
		}
		else if( dfs_nono(x,y+1) )
			{ --stack_size; return 1;}// correct
		
		// not correct restore the map
		g = no;

		g.map[x][y++]=-1;// so it is -1
		smart_solve();
		if( g.isdone())
			if( iscorrect())
				{--stack_size;return 1;}
			else
				{--stack_size;return 0;}// previous is wrong
	}
}

bool solve()
{
	smart_solve();
	if( !gram[0].isdone() )
	{
//		gram[0].OUTPUT();
//		puts("goDFS");
		stack_size =0 ;
		if( !dfs_nono(0,0) )
			return 0;
	}
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
