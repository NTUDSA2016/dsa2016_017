#include<bits/stdc++.h>

int main()
{
	for(int i=1;i<=38;++i)
	{
		char c[1000];
		sprintf(c,"curl http://mirlab.org/jang/courses/dsa/homework/2016/hw03/random_30x30/random_30x30_%02d.txt > ran%02d.txt",i,i);
		system(c);
	}

