#include<bits/stdc++.h>
using std::string;
using std::cout;using std::cin;

struct rowdata
{
	int user,item,time,result;

};
bool compu(const  rowdata &a,const rowdata &b)
{
	return a.user < b.user;
}
bool compi(const  rowdata &a,const rowdata &b)
{
	return a.item < b.item;
}
bool compiu(const  rowdata &a,const rowdata &b)
{
	if( a.user != b.user)
		return a.user < b.user;
	return a.time < b.time;
}
bool compit(const  rowdata &a,const rowdata &b)
{
	if( a.item !=  b.item)
		return a.item < b.item;
	return a.time < b.time;
}
bool compui(const  rowdata &a,const rowdata &b)
{
	if( a.user != b.user)
		return a.user < b.user;
	return a.item < b.item;
}
bool compuit(const  rowdata &a,const rowdata &b)
{
	if( a.user != b.user)
		return a.user < b.user;
	if( a.item != b.item)
		return a.item < b.item;
	return a.time < b.time;
}

#define VI std::vector<int> 
#define M 73209277

class mydata
{
private:
	rowdata* vrowdata;
	//i don't use pointer because it's slower 
	std::vector<rowdata> sortui ,sorti, sortiu;// user item time 
	std::unordered_set<int> issortu;
	
	std::unordered_map< int, VI > isaccept;
	std::unordered_map<int,int> userhold;
	VI hold;
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


void mydata::quickin(rowdata *d,char *ch)
{
	int t=0,now=0;
	for(int i=0;ch[i];++i)
		switch(ch[i])
		{
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				t = t*10 + ch[i]-'0';
				break;
			case '-':
				t=-1;++i;break;
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
	filein(s);
//	printf("%f file ok\n",((float)clock()/CLOCKS_PER_SEC));

	sortui.resize(M);
	std::copy(vrowdata,vrowdata+M,sortui.begin());
	std::stable_sort(sortui.begin(),sortui.end(),compui);
//	printf("%f ui ok\n",((float)clock()/CLOCKS_PER_SEC));
	
	sorti.resize(M);
	std::copy(vrowdata,vrowdata+M,sorti.begin());
	std::stable_sort(sorti.begin(),sorti.end(),compi);
//	printf("%f i ok\n",((float)clock()/CLOCKS_PER_SEC));
	
	issortu.clear();
	sortiu.resize(M);
	std::copy(sorti.begin(),sorti.end(),sortiu.begin());
//	printf("%f iu ok\n",((float)clock()/CLOCKS_PER_SEC));
	
	isaccept.clear();
	userhold.clear();
	hold.clear();
	finduserhold();
//	printf("%f hold ok\n",((float)clock()/CLOCKS_PER_SEC));

//	exploid();
//	printf("%f exploid ok\n",((float)clock()/CLOCKS_PER_SEC));
}

void mydata::finduserhold()
{
	int chold=1,m=M-1;
	for(int i=0;i<m;++i)
		if ( sortui[i].user != sortui[i+1].user )	
		{
			userhold[ sortui[i].user ]= chold;
			hold.push_back( chold );
			chold=1;
		}
		else if( compuit(sortui[i] ,sortui[i+1]) )// prevent repeat
			++chold;
	userhold[ sortui[m].user]=chold;
	hold.push_back( chold );

	std::sort(hold.begin(),hold.end());//increasing

}

void mydata::exploid()
{
	VI v;
	int it=-1;
	for(int i=0;i<M;++i)
		if( sorti[i].item != it )
		{
			it = sorti[i].item;
			ratio(it,i);
			findtime_item(it,v);
		}
}


int mydata::accept(int&u, int&i, int &t)// if the value are correct
{
	rowdata here{u,i,t,0} ;
	auto s = std::lower_bound(sortui.begin(),sortui.end(),here,compuit);
	if( s==sortui.end() || compuit(here,*s) )
		return 0;
	else
		return (s)->result;
}

VI mydata::items(int &u1,int& u2)
{
	rowdata here{u1,0,0,0} ;
	auto s1 = std::lower_bound(sortui.begin(),sortui.end(),here,compui);
	here.user = u2;
	auto s2 = std::lower_bound(sortui.begin(),sortui.end(),here,compui);
	
	VI ans;
	int re=-1;// for repeat item
	// beacuse item is sorted
	while( s1<sortui.end() && (s1)->user == u1 && s2<sortui.end() && (s2)->user == u2)
		if( (s1)->item ==re )
			++s1;
		else if( (s2)->item ==re )
			++s2;
		else if( (s1)->item < (s2)->item )
			++s1;
		else if( (s1)->item > (s2)->item )
			++s2;
		else
		{
			ans.push_back(re = (s1)->item);
			++s1;++s2;
		}
	return ans;
}

VI mydata::users(int &i1,int &i2,int &t1,int &t2)
{
	if(t2==INT_MAX)--t2;//prevent overflow
	std::unordered_set<int> set;
	rowdata here{0,i1,t1,0};
	auto s = std::lower_bound(sorti.begin(),sorti.end(),here,compit);
	here.time = t2+1;
	while(s<sorti.end() && compit(*s,here))
		set.insert((s++)->user);
	
	VI ans;
	here={0,i2,t1,0};
	s = std::lower_bound(sorti.begin(),sorti.end(),here,compit);
	here.time = t2+1;
	while(s<sorti.end() && compit(*s,here))
	{
		if(set.count((s)->user))
		{
			ans.push_back((s)->user);
			set.erase((s)->user);//prevent repeat user
		}
		++s;
	}

	std::sort(ans.begin(),ans.end());
	return ans;
}

VI mydata::ratio(int &it,int &thold)
{
	VI &vt = isaccept[it];

	if(!vt.size())
	{
		std::unordered_set<int> set;
		rowdata here{0,it,0,0};
		auto s = std::lower_bound(sorti.begin(),sorti.end(),here,compi);
		while( s<sorti.end() && (s)->item == it)
		{
			if( (s)->result == 1 )
			   set.insert( (s)->user );
			++s;
		}
		vt.resize( set.size() );
		int j=0;
		for(int i:set)
			vt[j++] = userhold[i];
		std::sort(vt.begin(),vt.end());//increase

		if(!vt.size())vt.push_back(0);//at least one
	}

	VI ans(2);
	ans[0]= vt  .end()-std::upper_bound(vt  .begin(),vt  .end(),thold);
	ans[1]= hold.end()-std::upper_bound(hold.begin(),hold.end(),thold);
	return ans;
}




VI mydata::findtime_item(int& it,VI& us)
{
	std::unordered_set<int> set;
	rowdata here{0,it,0,0};
	auto is = std::lower_bound(sortiu.begin(),sortiu.end(),here,compi),
		 ie = std::upper_bound(sortiu.begin(),sortiu.end(),here,compi);
	
	if(!issortu.count(it)){ // is sorted ?
		std::stable_sort(is,ie,compu);
		issortu.insert(it);
	}

	std::sort(us.begin(),us.end());// remove depulicated
	for(int &i:us)
	{
		if( is == ie)
			break;
		else if((is)->user>i)
			continue;
		else if((is)->user < i)//!=
		{
			here.user = i ;
			is = std::lower_bound(is,ie,here,compu);
		}
		while(is<ie && (is)->user==i )
			set.insert( (is++)->time );
	}

	VI ans(set.begin(),set.end());
	std::sort(ans.begin(),ans.end());
	return ans;
}

void display(VI ans)
{
	for(int& i:ans)
		printf("%d\n",i);
	if(!ans.size())
		puts("EMPTY");
}

VI myline()
{
	VI ans;
	string ch; 
	getline(std::cin,ch);
	ch.push_back(' ');// for the last one
	int t=0,n=ch.size();
	for(int i=0;i<n;++i)
		switch(ch[i])
		{
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				t = t*10 + ch[i]-'0';
				break;
			default:
			{
				if(t)//because t must be correct
					ans.push_back(t);
				t=0;
			}
		}
	return ans;
}



int main()
{
//	mydata my;
	mydata my("/tmp2/KDDCUP2012/track1/rec_log_train.txt");
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
//				puts("accept");
				scanf("%d%d%d",&u,&i,&t);
				printf("%d\n",my.accept(u,i,t));
				break;
			}
			case 1:
			{
				int u1,u2;
//				puts("items");
				scanf("%d%d",&u1,&u2);
				display( my.items(u1,u2) );
				break;
			}
			case 2:
			{
				int i1,i2,t1,t2;
//				puts("users");
				scanf("%d%d%d%d",&i1,&i2,&t1,&t2);
				display( my.users(i1,i2,t1,t2) );
				break;
			}
			case 3:
			{
				int i,h;
//				puts("ratio");
				scanf("%d%d",&i,&h);
				VI ans = my.ratio(i,h);
				printf("%d/%d\n",ans[0],ans[1]);
				break;
			}
			case 4:
			{
				int i;
//				puts("findtime_item");
				scanf("%d",&i);
				VI us = myline();
				display( my.findtime_item(i,us) );
				break;
			}
		}
		}
	}
	return 0;
}
