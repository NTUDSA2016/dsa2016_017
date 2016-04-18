#include<bits/stdc++.h>

struct Stock
{
	int id,client,action,price,count;

	bool in()
	{
		return ~scanf("%d%d%d%d%d",&id,&client,&action,&price,&count);
	}
	
	bool operator < (const Stock &b) const
	{
		return price < b.price ;
	}
};

#define Vstock  std::vector<Stock>
#define PQstock std::priority_queue<Stock>

int bidid=0;

void calbid(Stock &a,Stock &b)//buy sell
{
	int c = std::min( a.count, b.count);
	a.count -= c;
	b.count -= c;
	printf("%d %d %d %d %d\n",bidid++,a.client,b.client,std::abs(b.price),c);
}


void bid()
{
	PQstock q1,q2;
	Stock tmp;
	while( tmp.in() )
	{
		if(tmp.action == 2 || tmp.count==0)
			continue; // nothing
		// find bid 
		
		if( tmp.action == 0 )// buy
		{
			while( tmp.count && q2.size() && -q2.top().price <= tmp.price )
			{
				Stock q = q2.top();q2.pop();
				calbid(tmp,q);
				if(q.count )// nozero
					q2.push(q);
			}
		}
		
		else if( tmp.action == 1 )// sell
		{
			while( tmp.count && q1.size() && q1.top().price >= tmp.price )
			{
				Stock q = q1.top();q1.pop();
				calbid(q,tmp);
				if(q.count)// nozero
					q1.push(q);
			}
		}

		// put to PQ
		if( tmp.count == 0 )// no zero
			continue;

		if( tmp.action == 0)// buy
			q1.push(tmp);
		else if( tmp.action == 1)//sell
		{
			tmp.price *= -1;
			q2.push(tmp);
		}
	}
}


int main()
{
	bidid=0;
	bid();

	return 0;
};
