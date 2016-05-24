#include<bits/stdc++.h>

#define VD std::vector<double>

std::vector<int> ans;
double distance(VD &vn,VD &vm)
{
	int v[vm.size()][vn.size()];
	double dp[2][vn.size()];
	int now = 1;

	double sum=0;
	for(int i=0;i<vn.size();++i)
	{
		dp[0][i] = sum +=  std::abs(vn[i]-vm[0]);
		v [0][i]=0;
	}

	double min=dp[0][vn.size()-1];
	for(int j=1;j<vm.size()&&j<vn.size();++j,now^=1)
	{
		dp[now][j] = dp[now^1][j-1] + std::abs(vn[j]-vm[j]);
		v [  j][j] = j;
		for(int i=j+1;i<vn.size();++i)
			if(dp[now^1][i-1] <= dp[now  ][i-1])
			{
				dp[now][i] = dp[now^1][i-1] + std::abs(vn[i]-vm[j]);
				v [  j][i] = i ;
			}
			else
			{
				dp[now][i] = dp[now  ][i-1] + std::abs(vn[i]-vm[j]);
				v [  j][i] = v [j    ][i-1] ;
			}
		if( min > dp[now][vn.size()-1] )
		{
			min = dp[now][vn.size()-1];
			ans.clear();
			for(int jmp=vn.size()-1,k=j;k>=0 && jmp>=0;jmp = v[k][jmp]-1,--k)
				ans.push_back(v[k][jmp]);
		}
	}
	return min;
}


int main()
{
	int n,m;
	VD vn,vm;
	scanf("%d",&n);
	for(int i=0;i<n;++i)
	{
		double x ;scanf("%lf",&x);
		vn.push_back(x);
	}
	scanf("%d",&m);
	for(int i=0;i<m;++i)
	{
		double x ;scanf("%lf",&x);
		vm.push_back(x);
	}

	printf("%f\n",distance(vn,vm));
	for(auto i=ans.rbegin();i!=ans.rend();++i)
		printf("%d ",*i);
	puts("");

	return 0;
}

