#include<bits/stdc++.h>
//46sec for linear
//time is increasing
//94 for one sort
//218 for four sort
//145 for two sort
// 560 for uit sort
// 160 for stable rowdata 1 sort
// max 120 avg 29 per time
// max 14456 avg 52 per user
// max 8375857 avg 15543.370913 per item
using std::string;
using std::cout;using std::cin;

struct rowdata
{
	int user,item,time,result;
};

#define VI std::vector<int> 
//#define M 73209277
#define M 10000

class mydata
{
private:
	rowdata* vrowdata;
	std::vector<rowdata*> sortui ,sorti, sortiu;// user item time 
	std::set<int> issortu;
	
	std::map< int, VI > isaccept;
	std::map<int,int> userhold;
	std::vector< std::pair<int,int> > hold;
	void finduserhold();
	
	void filein(string &s);
	void quickin(rowdata*,char *);
	void exploid();// it is test

public:
	
	//init
	mydata(string);
	~mydata(){delete[] vrowdata;};

	int accept(int&,int&,int&);//0 not found 
	VI items(int&,int&);
	VI users(int &,int&,int&,int&);
	VI ratio(int&,int&);
	VI findtime_item(int&,VI&);

};

inline bool compu( rowdata *a, rowdata *b)
{
	return a->user < b->user;
}
inline bool compi( rowdata *a, rowdata *b)
{
	return a->item < b->item;
}
inline bool comput( rowdata *a, rowdata *b)
{
	if( a->user != b->user)
		return a->user < b->user;
	return a->time < b->time;
}
inline bool compit( rowdata *a, rowdata *b)
{
	if( a->item !=  b->item)
		return a->item < b->item;
	return a->time < b->time;
}
inline bool compui( rowdata *a, rowdata *b)
{
	if( a->user != b->user)
		return a->user < b->user;
	return a->item < b->item;
}
inline bool compuit( rowdata *a, rowdata *b)
{
	if( a->user != b->user)
		return a->user < b->user;
	if( a->item != b->item)
		return a->item < b->item;
	return a->time < b->time;
}

void mydata::quickin(rowdata *d,char *ch)
{
	int t=0,now=0;
	for(int i=0;;++i)
		switch(ch[i])
		{
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				t = t*10 + ch[i]-'0';
				break;
			case '-':
				t=-1;++i;break;
			case 0:break;
			default:
			{
				switch(now)
				{
					case 0:d->user=t;break;
					case 1:d->item=t;break;
					case 2:d->result=t;break;
					case 3:d->time=t;return ;break;
				}
				t=0;
				++now;
			}
		}
}

void mydata::filein(string &s)
{
	FILE *f=fopen(s.c_str(),"r");
	if(f==NULL)
	{
		puts("throw file error");
		return ;
	}
	int j=0;
	char ch[100];
	vrowdata = new rowdata [M];
	while( fgets(ch,100,f)  )
		quickin(vrowdata+j,ch),
		++j;
	if(j!=M)
		puts("throw M fault");
	fclose(f);
};

#define mybind(a) std::bind(&a,this,std::placeholders::_1,std::placeholders::_2)
mydata::mydata(string s="data.txt") 
{
	issortu.clear();
	filein(s);
	printf("%f file ok\n",((float)clock()/CLOCKS_PER_SEC));

	sortui.resize(M);
	sorti.resize(M);
	sortiu.resize(M);
	for(int i=0;i<M;++i)
		sortui[i] = sorti[i] = vrowdata+i;

	// to my data
	std::stable_sort(sortui.begin(),sortui.end(),compui);
	printf("%f u ok\n",((float)clock()/CLOCKS_PER_SEC));
	std::stable_sort(sorti.begin(),sorti.end(),compi);
	printf("%f i ok\n",((float)clock()/CLOCKS_PER_SEC));

	std::copy(sorti.begin(),sorti.end(),sortiu.begin());
	printf("%f iu ok\n",((float)clock()/CLOCKS_PER_SEC));
// it doesn't quicker
//	std::copy(sorti.begin(),sorti.end(),sortui.begin());
//	std::stable_sort(sortui.begin(),sortui.end(),compu);
//	printf("%f u ok\n",((float)clock()/CLOCKS_PER_SEC));
	isaccept.clear();
	userhold.clear();
	hold.clear();
	finduserhold();
	printf("%f hold ok\n",((float)clock()/CLOCKS_PER_SEC));

//	exploid();
//	printf("%f exploid ok\n",((float)clock()/CLOCKS_PER_SEC));
}

void mydata::finduserhold()
{
	int chold=1,m=M-1;
	for(int i=0;i<m;++i)
		if ( sortui[i]->user != sortui[i+1]->user )	
		{
			userhold[ sortui[i]->user ]= chold;
			hold.push_back( std::make_pair(chold,1) );
			chold=1;
		}
		else if( compuit(sortui[i] ,sortui[i+1]) )// prevent repeat
			++chold;
	userhold[ sortui[m]->user]=chold;
	hold.push_back( std::make_pair(chold,1) );

	std::sort(hold.rbegin(),hold.rend());//decreasing

	m = hold.size();
	int now=0;
	for(int i=1;i<m;++i)
		if( hold[now].first == hold[i].first )
			hold[now].second += hold[i].second;
		else
		{
			if(now)
				hold[now].second += hold[now-1].second;
			++now;
		}
	if(now)//the last one
		hold[now].second += hold[now-1].second;
	hold.resize(now+1);

}


void mydata::exploid()
{
	VI v;
	int it=-1;
	for(int i=0;i<M;++i)
		if( sorti[i]->item != it )
		{
			it = sorti[i]->item;
			ratio(it,i);
			findtime_item(it,v);
		}
}


int mydata::accept(int&u, int&i, int &t)// if the value are correct
{
	rowdata here{u,i,t,0} ;
	return  (*std::lower_bound(sortui.begin(),sortui.end(),&here,compuit))->result;

}

VI mydata::items(int &u1,int& u2)
{
	rowdata here{u1,0,0,0} ;
	auto s1 = std::lower_bound(sortui.begin(),sortui.end(),&here,compui);
	here.user = u2;
	auto s2 = std::lower_bound(sortui.begin(),sortui.end(),&here,compui);
	
	VI ans;
	int re=-1;// for repeat item
	// beacuse item is sorted
	while( s1<sortui.end() && (*s1)->user == u1 && s2<sortui.end() && (*s2)->user == u2)
		if( (*s1)->item ==re )
			++s1;
		else if( (*s2)->item ==re )
			++s2;
		else if( (*s1)->item < (*s2)->item )
			++s1;
		else if( (*s1)->item > (*s2)->item )
			++s2;
		else
		{
			ans.push_back(re = (*s1)->item);
			++s1;++s2;
		}
	return ans;
}


VI mydata::users(int &i1,int &i2,int &t1,int &t2)
{
	if(t2==INT_MAX)--t2;//prevent overflow
	std::set<int> set;
	rowdata here{0,i1,t1,0};
	auto s = std::lower_bound(sorti.begin(),sorti.end(),&here,compit);
	here.time = t2+1;
	while(s<sorti.end() && compit(*s,&here))
		set.insert((*s++)->user);
	
	VI ans;
	here={0,i2,t1,0};
	s = std::lower_bound(sorti.begin(),sorti.end(),&here,compit);
	here.time = t2+1;
	while(s<sorti.end() && compit(*s,&here))
	{
		if(set.count((*s)->user))
		{
			ans.push_back((*s)->user);
			set.erase((*s)->user);// repeat user
		}
		++s;
	}

	std::sort(ans.begin(),ans.end());
	return ans;
}

#define tget(a,b) (std::get<b>(a))
VI mydata::ratio(int &it,int &thold)
{
	VI &vt = isaccept[it];

	if(!vt.size())
	{
		rowdata here{0,it,0,0};
		auto s = std::lower_bound(sorti.begin(),sorti.end(),&here,compi);
		while( s<sorti.end() && (*s)->item == it)
			if( (*s)->result == 1 )
			   vt.push_back( (*s)->user );
		if(!vt.size())
			vt.push_back(0);	//at least 1
		std::sort(vt.begin(),vt.end());//prevent repeat
		vt.resize( std::unique(vt.begin(),vt.end()) - vt.begin() ) ;
	}


	if(thold == INT_MAX)//prevent overflow
		--thold;
	VI ans(2);
	ans[1]= std::lower_bound(hold.rbegin(),hold.rend(),std::make_pair(thold+1,0))->second;
	ans[0]=0;
	for(int &i:vt)
		if( userhold[i] > thold )
			++ans[0];
	return ans;
}




VI mydata::findtime_item(int& it,VI& us)
{
	std::set<int> set;
	rowdata here{0,it,0,0};
	auto is = std::lower_bound(sortiu.begin(),sortiu.end(),&here,compi),
		 ie = std::upper_bound(sortiu.begin(),sortiu.end(),&here,compi);
	
	if(!issortu.count(it)){ // is sorted ?
		std::stable_sort(is,ie,compu);
		issortu.insert(it);
	}

	for(int &i:us)
	{
		here.user = i ;
		auto s = std::lower_bound(is,ie,&here,compu);
		while(s<ie && (*s)->user==i )
			set.insert( (*s++)->time );
	}

	return VI(set.begin(),set.end());
}


void display(VI ans)
{
	for(int& i:ans)
		printf("%d ",i);
	if(!ans.size())
		printf("EMPTY");
	puts("");
}


int main()
{
	mydata my;
//	mydata my("/tmp2/KDDCUP2012/track1/rec_log_train.txt");
	int all;
	string s[5]={"accept","items","users","ratio","findtime_item"};
	while( ~scanf("%d",&all) )
	{
		while(all--){
			string ins;std::cin>>ins;
			int mode;
			for(mode=0;mode<5;++mode)
				if(s[mode]==ins)
					break;
			if(mode==5)//my input 0,1,2,3,4
				mode = ins[0] - '0' ;
		switch(mode)
		{
			case 0:
			{
				int u,i,t;
				puts("accept");
				scanf("%d%d%d",&u,&i,&t);
				printf("%d\n",my.accept(u,i,t));
				break;
			}
			case 1:
			{
				int u1,u2;
				puts("items");
				scanf("%d%d",&u1,&u2);
				display( my.items(u1,u2) );
				break;
			}
			case 2:
			{
				int i1,i2,t1,t2;
				puts("users");
				scanf("%d%d%d%d",&i1,&i2,&t1,&t2);
				display( my.users(i1,i2,t1,t2) );
				break;
			}
			case 3:
			{
				int i,h;
				puts("ratio");
				scanf("%d%d",&i,&h);
				VI ans = my.ratio(i,h);
				printf("%d/%d\n",ans[0],ans[1]);
				break;
			}
			case 4:
			{
				int i;
				puts("findtime_item");
				VI us;
				scanf("%d",&i);
				string s;
				getline(cin,s); // read a line of interger
				std::istringstream iss(s);
				int x;
				while(iss>>x)
					us.push_back(x);
					
				display( my.findtime_item(i,us) );
				break;
			}
		}
		}
	}
	return 0;
}
