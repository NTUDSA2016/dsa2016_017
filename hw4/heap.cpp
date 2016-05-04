#include<bits/stdc++.h>

#define type int
struct heapdata
{
	int index;
	type dat;
};

class heap
{
	private:
		std::vector<heapdata*> v;
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

		void print()
		{
			for(int i=1;i<v.size();++i)
				printf("%d ",v[i]->dat);
			puts("");
		}
};

int heap::size()
{
	return v.size()-1;
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
	delete v.back();
	v.pop_back();
}

heap::heap()
{
	for(int i=1;i<v.size();++i)
		delete v[i];
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
	goup( v.size()-1 );
}

type& heap::top()// be careful
{
	return v[1]->dat;// from 1
}

void heap::remove(int i)
{
	swap(i,v.size()-1);
	removeback();
	if( i == v.size()-1)
		return ;
	godown(i);
	goup(i);  // it may be go up 
}


int main()
{
	heap h;
	while(1)
	{
		int a,b;
		scanf("%d",&a);
		if(a)
		{
			scanf("%d",&b);
			h.push(b);
		}
		else
			h.pop();
		h.print();
	}
}
