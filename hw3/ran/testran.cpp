#include<bits/stdc++.h>

int main()
{
	for(int i=1;i<=38;++i)
	{
		char c[1000];
		sprintf(c,"./out < ran/ran%02d.txt | grep error",i);
		system(c);
	}
}

