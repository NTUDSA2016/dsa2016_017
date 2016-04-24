#include<bits/stdc++.h>

std::vector<int> pre,post;

void in(std::vector<int> &v)
{
	v.clear();
	std::string s;
	getline(std::cin,s);
	std::istringstream iss(s);
	int t;
	while( iss >> t)
		v.push_back(t);

}

std::map<int,int> map;

int N,pe;

void compare(int tree,int po1,int po2)
{
//	printf("%d %d %d\n",tree,po1,po2);
	if( po1 >= po2 || pe==N)
		return ;
	for(int i=po1;i<po2;++i)
		if( pre[pe] == post[i] )
		{
			map[tree] = pre[pe++];
			compare(tree*2,po1,i);
			compare(tree*2+1,i+1,po2);
			return ;
		}
	puts("error");
}

int main()
{
	in(pre);
	in(post);

	N = pre.size();
	pe = 0;

	compare(1,0,N);

	int n = map.rbegin()->first;
	printf("-1 ");
	for(int i=1;i<=n;++i)
	{
		if( map.count(i))
			printf("%d ",map[i]);
		else
			printf("-1 ");
	}
	puts("");

	return 0;
}
