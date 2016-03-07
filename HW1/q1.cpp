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
	for(int i=0;i<n;++i)
	{
		if(std::fabs(q[i][i])<1e-7)
			puts("error");
		for(int j=0;j<n;++j)
			if(i!=j)
			{
				double r = q[j][i] / q[i][i];
				for(int k=i;k<n;++k) // it doesnot need to from 0
					q[j][k] -= q[i][k]*r;
				for(int k=0;k<n;++k)
					ans[j][k] -= ans[i][k]*r;

			}
	}
	for(int i=0;i<n;++i)
	{
		double k=q[i][ i ];
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
//	printans();
	return 0;
}
