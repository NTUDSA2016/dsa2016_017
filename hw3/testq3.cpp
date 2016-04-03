#include<bits/stdc++.h>

#define MM 101
#define VI std::vector<int>
using std::string;

char *st_arr; 
int st_wh[MM],st_m;
std::vector< std::pair<int,int> > stat[2];
VI st_lim;

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
		copyout(i);
		to_statical();
		for(int j=0;j<stat[1].size();++j)
			row[i].push_back( stat[1][j].second - stat[1][j].first);

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
		char &now = st_arr[i];
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
	int n = rand()%50+1;
	int m = rand()%50+1;

	gram[0].init(n,m);
	gram[1].init(m,n);

	int k = rand()%(n*m);
	while(k--)
	{
		int  x = rand()%n , y= rand()%m ;
		gram[0].map[x][y] = 1;
	}
	gram[0].limit_input();
	gram[1].copymap(gram[0]);
	gram[1].limit_input();

	gram[0].init(n,m);
	gram[1].init(m,n);
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

int late=0,wrong=0;
void filein(int a)
{
	if(late>42 || wrong>42)
		exit(0);
	char c[100];
	if(a==0)
		sprintf(c,"late%d",late++);
	else 
		sprintf(c,"wrong%d",wrong++);
	FILE *f = fopen(c,"w");
	fprintf(f,"%d %d\n",gram[0].n,gram[0].m);
	for(int k=0;k<2;++k)
		for(int i=0;i<gram[k].n;++i,fprintf(f,"\n"))
			for(int &j:gram[k].row[i])
				fprintf(f,"%d ",j);
	fclose(f);
}

float t =((float)clock()/CLOCKS_PER_SEC);
bool dfs_nono(int x,int y)
{
	if( ((float)clock()/CLOCKS_PER_SEC) - t > 2 )
	{
		filein(0);
		return 1;
	}
//	static int ti=0;
//	printf("%d\n",ti++);
	nono &g = gram[0];
	nono no;
	while(1)
	{
		// find the 0 in the map
		while(y>=g.m || x>=g.n || g.map[x][y]!=0)
		{
			++y;
			if(y>=g.m)
				++x,y=0;
			if(x>=g.n)
				return 0;// because if it is not find 0 it is correct
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
				return 1;
		}
		else if( dfs_nono(x,y+1) )
			return 1;// correct
		
		// not correct restore the map
		g = no;

		g.map[x][y++]=-1;// so it is -1
		smart_solve();
		if( g.isdone())
			if( iscorrect())
				return 1;
			else
				return 0;// previous is wrong
	}
}


bool solve()
{
	smart_solve();
	if( !gram[0].isdone() )
	{
//		gram[0].OUTPUT();
//		puts("goDFS");
		if( !dfs_nono(0,0) )
			return 0;
	}
	return iscorrect();
}


int main()
{
//	test();
	srand(time(NULL));

	int ti=0;

	while(1)
	{
		INPUT();
//		filein(2);
		t =((float)clock()/CLOCKS_PER_SEC);
		int k  = solve();
//		gram[0].OUTPUT();
		if( ((float)clock()/CLOCKS_PER_SEC) - t <= 2 && !k )
			filein(1);
		printf("%d\n",ti++);
	}
};
