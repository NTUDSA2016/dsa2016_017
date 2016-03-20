#include<bits/stdc++.h>
using std::string;
using std::cout;using std::cin;

struct rowdata
{
	int user,item,time,result;
};

#define VI std::vector<int> 
class mydata
{
private:
	std::vector<rowdata> vrowdata;
	int n;//size
	VI uit;// user item time 
	VI itu;
	VI iut;
	
	void filein(string &s);
	bool uitcomp(const int &,const int &)const;
	bool itucomp(const int &,const int &)const;
	bool iutcomp(const int &,const int &)const;

public:
	
	//init
	mydata(string);

	int accept(int&,int&,int&);//0 not found 
	VI items(int&,int&);
	VI users(int &,int&,int&,int&);
	VI ratio(int&,int&);
	VI findtime_item(int&,VI&);

};

void mydata::filein(string &s)
{
	FILE *f=fopen(s.c_str(),"r");
	if(f==NULL)
	{
		puts("throw file error");
		return ;
	}
	vrowdata = std::vector<rowdata>() ;
	int u,i,r,t;
	while( ~fscanf(f,"%d%d%d%d",&u,&i,&r,&t) )
		vrowdata.push_back( rowdata{u,i,t,r} );
	fclose(f);
};

#define mybind(a) std::bind(&a,this,std::placeholders::_1,std::placeholders::_2)
mydata::mydata(string s="data.txt") 
{
	filein(s);
	n = vrowdata.size();
	vrowdata.resize(n+1);// a tmp on the back

	uit.resize(n);
	itu.resize(n);
	iut.resize(n);
	for(int i=0;i<n;++i)
		uit[i]=itu[i]=iut[i]=i;

	// to my data
	std::sort(uit.begin(),uit.end(),mybind(mydata::uitcomp));
	std::sort(itu.begin(),itu.end(),mybind(mydata::itucomp));
//	for(int i=0;i<itu.size();++i)
//		printf("%d %d %d\n",vrowdata[itu[i]].user,vrowdata[itu[i]].item,vrowdata[itu[i]].time);
	std::sort(iut.begin(),iut.end(),mybind(mydata::iutcomp));
}

bool mydata::uitcomp(const int &a,const int &b)const
{
	const rowdata &ra = vrowdata[a],
				  &rb = vrowdata[b];

	if(ra.user != rb.user)
		return ra.user < rb.user;
	if(ra.item != rb.item )
		return ra.item < rb.item;
	if(ra.time != rb.time)
		return ra.time < rb.time;
	return 0;
}
bool mydata::itucomp(const int &a,const int &b)const
{
	const rowdata &ra = vrowdata[a],
				  &rb = vrowdata[b];

	if(ra.item != rb.item )
		return ra.item < rb.item;
	if(ra.time != rb.time)
		return ra.time < rb.time;
	if(ra.user != rb.user)
		return ra.user < rb.user;
	return 0;
}
bool mydata::iutcomp(const int &a,const int &b)const
{
	const rowdata &ra = vrowdata[a],
				  &rb = vrowdata[b];

	if(ra.item != rb.item )
		return ra.item < rb.item;
	if(ra.user != rb.user)
		return ra.user < rb.user;
	if(ra.time != rb.time)
		return ra.time < rb.time;
	return 0;
}

int mydata::accept(int&u, int&i, int &t)
{
	vrowdata.back()=  rowdata{u,i,t,0} ;
	auto now = std::lower_bound(uit.begin(),uit.end(),n,mybind(mydata::uitcomp));

	if( now == uit.end() || uitcomp(n,*now) || uitcomp(*now,n)  )
		return 0;
	else
		return vrowdata[ *now ].result;
}

VI mydata::items(int &u1,int& u2)
{
	VI ans;
	vrowdata.back() =  rowdata{u1,0,0,0} ;
	auto s1= std::lower_bound(uit.begin(),uit.end(),n,mybind(mydata::uitcomp));
	vrowdata.back().user=u2;
	auto s2= std::lower_bound(uit.begin(),uit.end(),n,mybind(mydata::uitcomp));

	while( s1<uit.end() && s2<uit.end() &&vrowdata[*s1].user ==u1 && vrowdata[*s2].user==u2)
		if(vrowdata[*s1].item < vrowdata[*s2].item)
			++s1;
		else if(vrowdata[*s1].item > vrowdata[*s2].item)
			++s2;
		else
		{
			ans.push_back(vrowdata[*s1].item);
			++s1;++s2;
		}

	return ans;
}

VI mydata::users(int &i1,int &i2,int &t1,int &t2)
{
	VI ans;
	std::set<int> set;
	
	vrowdata.back() = rowdata{0,i1,t1,0};
	auto s1= std::lower_bound(itu.begin(),itu.end(),n,mybind(mydata::itucomp));
	vrowdata.back() = rowdata{0,i1,t2+1,0};
	while(s1<itu.end() && itucomp(*s1,n))
		set.insert(vrowdata[*s1++].user);
	
	vrowdata.back() = rowdata{0,i2,t1,0};
	auto s2= std::lower_bound(itu.begin(),itu.end(),n,mybind(mydata::itucomp));
	vrowdata.back() = rowdata{0,i2,t2+1,0};
	while(s2<itu.end() && itucomp(*s2,n))
	{
		if(set.count(vrowdata[*s2].user))
		{
			ans.push_back(vrowdata[*s2].user);
			set.erase(vrowdata[*s2].user);// repeat user
		}
		++s2;
	}

	std::sort(ans.begin(),ans.end());
	return ans;
}

VI mydata::ratio(int &it,int &hold)
{
	std::map<int,int> map;
	std::set<int> set;
	vrowdata.back() = rowdata{0,it,0,0};
	auto s= std::lower_bound(iut.begin(),iut.end(),n,mybind(mydata::iutcomp));
	while(s<iut.end() && vrowdata[*s].item == it)
	{
		++map[ vrowdata[*s].user ]  ;
		if( vrowdata[*s].result == 1)
			set.insert(vrowdata[*s].user);
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
	vrowdata.back() = rowdata{0,it,0,0};
	for(int &i:us)
	{
		vrowdata.back().user = i ;
		auto s= std::lower_bound(iut.begin(),iut.end(),n,mybind(mydata::iutcomp));
		while(s<iut.end() && vrowdata[*s].user==i && vrowdata[*s].item == it)
			set.insert( vrowdata[*s++].time );
	}

	return VI(set.begin(),set.end());
}

int main()
{
	mydata my;
//	mydata my("/tmp2/KDDCUP2012/track1/rec log train.txt");
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
