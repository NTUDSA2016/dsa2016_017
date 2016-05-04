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
		if( price !=  b.price)
			return price < b.price ;
		return id > b.id;
	}
};


#define type Stock
struct heapdata
{
	int index;
	type dat;
};

class heap
{
	private:
		std::vector<heapdata*> v;
		std::unordered_map<int,heapdata*> map;
		void godown(int);
		void goup(int);
		void swap(int,int);
		void removeback();
	public:
		heap();
		void pop();
		void push(type);
		type& top();
		void remove(int);//remove the special one
		int size();

		void cancel(int );

};

int heap::size()
{
	return v.size()-1;
}

void heap::cancel(int i)
{
	if(map.count(i))
		remove( map[i]->index );
}

void heap::goup(int i)
{
	while(i>1 && v[i>>1]->dat < v[i]->dat )
	{
		swap( i>>1 , i);
		i = i>>1;
	}
}

void heap::godown(int i)
{
	while(i*2 < v.size())
	{
		int k = i<<1;
		if( k+1 < v.size() && v[k]->dat < v[k+1]->dat)
			 ++k;
		if( v[i]->dat < v[k]->dat)
		{
			swap(i,k);
			i = k;
		}
		else
			return ;
	}
}


void heap::swap(int a,int b)
{
	std::swap(v[a],v[b]);
	v[a]->index = a;
	v[b]->index = b;
}

void heap::removeback()
{
	map.erase( v.back()->dat.id); // only this is associate with type
	delete v.back();
	v.pop_back();
}

heap::heap()
{
	v.clear();
	v.push_back(NULL);// 0 no use
}

void heap::pop()
{
	remove(1);
}

void heap::push(type a)
{
	v.push_back( new heapdata{(int)v.size(),a} );
	map[a.id] = v.back();  // this assoicate with type
	goup( v.size()-1 );
}

type& heap::top()// be careful
{
	return v[1]->dat;// from 1
}

void heap::remove(int i)
{
	if( !i ) // 0 no use map not found
		return ;
	if( i == v.size()-1)// last one
	{
		removeback();
		return ;
	}
	swap(i,v.size()-1);
	removeback();
	godown(i);
	goup(i);
}

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
	heap q1,q2;
	Stock tmp;

	while( tmp.in() )
	{
		if( tmp.action == 2)
		{
			q1.cancel( tmp.price );
			q2.cancel( tmp.price );
			continue;
		}
		if( tmp.count==0)
			continue; // nothing
		// find bid 
		
		if( tmp.action == 0 )// buy
		{
			while( tmp.count && q2.size() && -q2.top().price <= tmp.price )
			{
				Stock& q = q2.top();
				calbid(tmp,q);
				if(!q.count)// zero
					q2.pop();
			}
		}
		
		else if( tmp.action == 1 )// sell
		{
			while( tmp.count && q1.size() && q1.top().price >= tmp.price )
			{
				Stock& q = q1.top();
				calbid(q,tmp);
				if(!q.count)// zero
					q1.pop();
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
