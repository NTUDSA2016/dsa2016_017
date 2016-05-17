#include<bits/stdc++.h>
#define ll long long 

// DP
int DP_ED[40][40];
int EditDistance(char *ca,char *cb)
{
	int Lena = strlen(ca),
		Lenb = strlen(cb);
	for(int j=0;j<=Lena;++j)
		DP_ED[0][j] = j ;	


	for(int i=1;i<=Lenb;++i)
	{
		DP_ED[i][0] = i;
		for(int j=1;j<=Lena;++j)
		{
			// add remove
			DP_ED[i][j] = std::min( DP_ED[i-1][j], DP_ED[i][j-1]) + 1;
			// same or replace
			DP_ED[i][j] = std::min( DP_ED[i  ][j  ], 
									DP_ED[i-1][j-1]+ (cb[i-1] != ca[j-1]) );
			// transport
			if( i>=2)// add 
				for( int k=j-1;k>=1 && DP_ED[i][k+1] == DP_ED[i][k]+1;--k)
					if( cb[i-1] == ca[k-1] && cb[i-2] == ca[j-1]  )
						DP_ED[i][j] = std::min( DP_ED[i][j], 
										DP_ED[i-2][k-1] + j-k );
			if( j>=2)// remove
				for( int k=i-1;k>=1 && DP_ED[k+1][j] == DP_ED[k][j]+1;--k)
					if( cb[k-1] == ca[j-1] && cb[i-1] == ca[j-2]  )
						DP_ED[i][j] = std::min( DP_ED[i][j], 
										DP_ED[k-1][j-2] + i-k );
		}
	}

/*debug
	for(int i=0;i<=Lenb;++i,puts(""))
		for(int j=0;j<=Lena;++j)
			printf("%2d ",DP_ED[i][j]);
*/
	return DP_ED[Lenb][Lena];
}

int main()
{
	char ca[40],cb[40];
	while(std::cin>> ca >> cb)
		printf("%d\n",EditDistance(ca,cb));
	return 0;
}

