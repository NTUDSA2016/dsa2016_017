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

#define PQstock std::priority_queue<Stock>
int tranid=0;

void calbid(Stock &a,Stock &b)//buy sell
{
	int c = std::min( a.count, b.count);
	a.count -= c;
	b.count -= c;
	printf("%d %d %d %d %d\n",tranid++,a.client,b.client,std::abs(b.price),c);
}

void bid()
{
	PQstock q1,q2;
	Stock tmp;
	std::unordered_set<int> cancel;

	while( tmp.in() )
	{
		if( tmp.action == 2)
		{
			cancel.insert( tmp.price );
			continue;
		}
		if( tmp.count==0)
			continue; // nothing
		// find bid 
		
		if( tmp.action == 0 )// buy
		{
			while( tmp.count && q2.size() && -q2.top().price <= tmp.price )
			{
				if( cancel.count( q2.top().id ) )
				{
					cancel.erase( q2.top().id);
					q2.pop();
					continue;// is canceled
				}
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
				if( cancel.count( q1.top().id ) )
				{
					cancel.erase( q1.top().id);
					q1.pop();
					continue;//is canceled
				}
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
			tmp.price *= -1;// because priority is biggest one
			q2.push(tmp);
		}
	}
}

int main()
{
	tranid=0;
	bid();

	return 0;
};
