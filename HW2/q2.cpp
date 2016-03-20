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
#define M 20000
class mydata
{
private:
	rowdata* vrowdata;
	std::vector<rowdata*> sortu ,sorti;// user item time 
	std::set<int> issortu;
	
	void filein(string &s);

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

void mydata::filein(string &s)
{
	FILE *f=fopen(s.c_str(),"r");
	if(f==NULL)
	{
		puts("throw file error");
		return ;
	}
	int u,i,r,t,j=0;
	vrowdata = new rowdata [M];
	while( ~fscanf(f,"%d%d%d%d",&u,&i,&r,&t) )
		vrowdata[j++] = ( rowdata{u,i,t,r} );
	fclose(f);
};

#define mybind(a) std::bind(&a,this,std::placeholders::_1,std::placeholders::_2)
mydata::mydata(string s="data.txt") 
{
	issortu= std::set<int>();
	filein(s);
	printf("%f file ok\n",((float)clock()/CLOCKS_PER_SEC));

	sortu.resize(M);
	sorti.resize(M);
	for(int i=0;i<M;++i)
		sortu[i] = sorti[i] = vrowdata+i;

	// to my data
	std::stable_sort(sortu.begin(),sortu.end(),compu);
	printf("%f u ok\n",((float)clock()/CLOCKS_PER_SEC));
	std::stable_sort(sorti.begin(),sorti.end(),compi);
	printf("%f i ok\n",((float)clock()/CLOCKS_PER_SEC));
}


int mydata::accept(int&u, int&i, int &t)
{
	rowdata here{u,i,t,0} ;
	auto now = std::lower_bound(sortu.begin(),sortu.end(),&here,comput);

	while( (*now)->user ==u && (*now)->time == t )
	{
		if( (*now)->item ==i)
			return (*now)->result;
		++now;
	}
	return 0;//not find

}

VI mydata::items(int &u1,int& u2)
{
	rowdata here{u1,0,0,0} ;
	auto s1 = std::lower_bound(sortu.begin(),sortu.end(),&here,compu);
	here.user=u2;
	auto s2 = std::lower_bound(sortu.begin(),sortu.end(),&here,compu);
	
	VI ans;

	while( s1<sortu.end() && s2<sortu.end() &&(*s1)->user ==u1 && (*s2)->user == u2)
		if((*s1)->item < (*s2)->item)
			++s1;
		else if((*s1)->item > (*s2)->item)
			++s2;
		else
		{
			ans.push_back((*s1)->item);
			++s1;++s2;
		}

	return ans;
}


VI mydata::users(int &i1,int &i2,int &t1,int &t2)
{
	VI ans;
	std::set<int> set;
	
	rowdata here{0,i1,t1,0};
	auto s = std::lower_bound(sorti.begin(),sorti.end(),&here,compit);
	here.time = t2+1;
	while(s<sorti.end() && compu(*s,&here))
		set.insert((*s++)->user);
	
	here={0,i2,t1,0};
	s = std::lower_bound(sorti.begin(),sorti.end(),&here,compit);
	here.time = t2+1;
	while(s<sorti.end() && compu(*s,&here))
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

VI mydata::ratio(int &it,int &hold)
{
	std::map<int,int> map;
	std::set<int> set;
	rowdata here{0,it,0,0};
	auto s = std::lower_bound(sorti.begin(),sorti.end(),&here,compi);
	while(s<sorti.end() && (*s)->item == it)
	{
		++map[ (*s)->user ]  ;
		if( (*s)->result == 1)//accept
			set.insert((*s)->user);
		++s;
	}

	VI ans(2);
	ans[0]=ans[1]=0;
	for( auto &i : map)
		if(i.second>hold)
		{
			++ans[1];
			if( set.count(i.first))
				++ans[0];
		}
	return ans;
}


VI mydata::findtime_item(int& it,VI& us)
{
	std::set<int> set;
	rowdata here{0,it,0,0};
	auto is = std::lower_bound(sorti.begin(),sorti.end(),&here,compi),
		 ie = std::upper_bound(sorti.begin(),sorti.end(),&here,compi);
	
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


int main()
{
	mydata my;
//	mydata my("/tmp2/KDDCUP2012/track1/rec_log_train.txt");
	int mode;
	while( ~scanf("%d",&mode) )
	{
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
				VI ans = my.items(u1,u2);
				for(int i:ans)
					printf("%d ",i);
				puts("");
				break;
			}
			case 2:
			{
				int i1,i2,t1,t2;
				puts("users");
				scanf("%d%d%d%d",&i1,&i2,&t1,&t2);
				VI ans = my.users(i1,i2,t1,t2);
				for(int i:ans)
					printf("%d ",i);
				puts("");
				break;
			}
			case 3:
			{
				int i,h;
				puts("radio");
				scanf("%d%d",&i,&h);
				VI ans = my.ratio(i,h);
				printf("%d/%d\n",ans[0],ans[1]);
				break;
			}
			case 4:
			{
				int i,u2;
				puts("findtime_items");
				int n;
				scanf("%d%d",&i,&n);
				VI us;
				for(int i=0;i<n;++i)
				{
					int x;scanf("%d",&x);
					us.push_back(x);
				}
					
				VI ans = my.findtime_item(i,us);
				for(int i:ans)
					printf("%d ",i);
				puts("");
				break;
			}
		}
	}
	return 0;
}
