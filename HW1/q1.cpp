#include<bits/stdc++.h>
using std::cin;using std::cout;using std::endl;

double que[201][201],q[201][201],ans[201][201];
int n;

void in()
{
	//in
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			scanf("%lf",&que[i][j]);
			q[i][j] = que[i][j];
		}

	//init
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			ans[i][j]= i==j;
}

void cal()
{
	char use[201];
	for(int i=0;i<n;++i)
		use[i]=-1;

	for(int i=0;i<n;++i)
	{
		int st=0;
		for(st=0;st<n;++st)
			if( use[st]==-1 && std::fabs(q[st][i]) > 1e-7)
				break;
		if(st==n)
			puts("error");// debug
		use[st]=i;

		for(int j=0;j<n;++j)
			if(st!=j)
			{
				double r = q[j][i] / q[st][i];
				for(int k=i;k<n;++k) // it doesnot need to from 0
					q[j][k] -= q[st][k]*r;
				for(int k=0;k<n;++k)
					ans[j][k] -= ans[st][k]*r;

			}
	}
	for(int i=0;i<n;++i)
	{
		double k=q[i][ use[i] ];
		for(int j=0;j<n;++j)
			ans[i][j] /= k ;
	}
}

double geterror()
{
	double max=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			double all=0;
			for(int k=0;k<n;++k)
				all += que[j][k] * ans[k][i] ;
			max = std::max( max , std::fabs(all - (i==j)));
		}
	return max;
}

double printans()
{
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			std::cout.precision(5);
			if(j)
				std::cout.width(11);
//			printf("%lf ",ans[i][j]);
			cout << ans[i][j] ; 
		}
		puts("");
	}
}


int main()
{
	cin >> n;
	in();
	cal();
	double d = geterror();
	cout << d << endl;
	printans();
	return 0;
}
