#include<bits/stdc++.h>

#define Stack std::stack<int>
#define VI std::vector<int>

int N ;
void permu(int now,VI v,Stack s)
{
	if(now > N )
	{
		while( s.size() )
		{
			v.push_back(s.top());
			s.pop();
		}
		for( auto &i : v)
			printf("%d ",i);
		puts("");
		return ;
	}

	s.push(now);
	permu(now+1,v,s);
	
	int k = s.top();s.pop();
	while( s.size() )
	{
		v.push_back(s.top());
		s.pop();
		s.push(now);
		permu(now+1,v,s);
		s.pop();

	}
}

int main()
{
	scanf("%d",&N);
	permu(1,VI(),Stack());
	return 0;
}
