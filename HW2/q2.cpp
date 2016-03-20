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
	std::set<int> set;
	rowdata here{u1,0,0,0} ;
	auto s = std::lower_bound(sortu.begin(),sortu.end(),&here,compu);
	
	while( s<sortu.end() && (*s)->user == u1)
		set.insert( (*s++)->item );

	VI ans;
	here.user=u2;
	s = std::lower_bound(sortu.begin(),sortu.end(),&here,compu);
	
	while( s<sortu.end() && (*s)->user == u2)
	{
		if (set.count( (*s)->item ))
		{
			ans.push_back((*s)->item);
			set.erase( (*s)->item);
		}
		++s;
	}

	std::sort(ans.begin(),ans.end());
	return ans;
}


VI mydata::users(int &i1,int &i2,int &t1,int &t2)
{
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


void display(VI ans)
{
	for(int i:ans)
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
				puts("radio");
				scanf("%d%d",&i,&h);
				VI ans = my.ratio(i,h);
				printf("%d/%d\n",ans[0],ans[1]);
				break;
			}
			case 4:
			{
				int i;
				puts("findtime_items");
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
