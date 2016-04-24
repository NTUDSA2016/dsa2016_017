#include<bits/stdc++.h>

struct BT
{
	std::vector<int> v;
	void in()
	{
		v.clear();
		std::string s;
		getline(std::cin,s);
		std::istringstream iss(s);
		int t;
		while( iss >> t)
			v.push_back(t);

	}

	void preorder(int now)
	{
		if(now>=v.size())
			return ;
		if(v[now]!=-1)
			printf("%d ",v[now]);
		preorder(now*2);
		preorder(now*2+1);
	}
	void inorder(int now)
	{
		if(now>=v.size())
			return ;
		inorder(now*2);
		if(v[now]!=-1)
			printf("%d ",v[now]);
		inorder(now*2+1);
	}
	void postorder(int now)
	{
		if(now>=v.size())
			return ;
		postorder(now*2);
		postorder(now*2+1);
		if(v[now]!=-1)
			printf("%d ",v[now]);
	}
};

int main()
{
	BT tree;
	tree.in();
	tree.preorder(1);
	puts("");
	tree.inorder(1);
	puts("");
	tree.postorder(1);
	puts("");
	return 0;
}
