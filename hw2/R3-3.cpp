#include<bits/stdc++.h>
using std::cin;
using std::cout;

void transmat(double **mat,int n)
{
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			std::swap(mat[i][j] , mat[j][i]);
}

int main()
{
	int n;
	cin>>n;
	
	double **mat = new double* [n];
	for(int i=0;i<n;++i)
	{
		mat[i]=  new double [n];
		for(int j=0;j<n;++j)
			cin>>mat[i][j];
	}

	transmat(mat,n);

	for(int i=0;i<n;++i,puts(""))
	{
		for(int j=0;j<n;++j)
			cout << mat[i][j] << ' ';
		delete[] mat[i];
	}
	delete mat;

	return 0;
}

