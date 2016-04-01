#include<bits/stdc++.h>
int c[100];
int& get(int i,int j)
{
	return c[(long long)i*(i-1)/2+j];
}
	
int main()
{
	get(3,2)=4;
	printf("%d\n",get(3,2));
	return 0;
}
