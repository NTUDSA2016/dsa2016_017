#HW2
## how to use
if you type `make` , it will compile it with O3
if you don't want and want to use O3 , type `make normal`
and type `make run` to run the program

##hand-write homework
##2.1.1
 Just need a tmp sizeof(float) to swap the data
 If your data is at `arr[i][j]` than swap with `arr[n-i][n-j]`
 In c++ just need a triangle side of the array
 ```
 for(int i=0;i<n;++i)
 	for(int j=0;j<i;++i) // a right up triangle 
		swap( arr[i][j] , arr[n-1-i][n-1-j]);
```
And the code you can find R3-3.cpp in my github in the hw2 tag

##2.1.2
use map the triangular matrix(i smaller than j) to an array
if the index is start at 0
that (i,j) map to ( i(i-1)/2+j ) when (i>0)

so if i smaller than n and the maxtrix is fully (j+1=i)
the size is n(n-1)/2

##2.2.2 my data struct
A big array ( vrowdata ) for store the original data
Two sorted array , one sorted by user and item , one sorted by items (n log n)
A copy array from item-sort (n)

A map for calculate the time of users recommad by system (n log n)
A array for sort recommand time (n log n) 

A map for store weather the item-sort array(copy one) is sorted by user
A map for store the vector of the recommand time of people who accept the item

It will cause about nlogn (stable sort)

Beacuse the original data is sorted by time , so my two array will sorted by time too.

##2.2.3 running time
On the workstation (2:00 linux9)

about (add up)
* 17 sec for read the file
* 74 sec for a array sorted by user and item
* 128 sec for a array sorted by item 
* 132 sec for calculate ratio
* 180 sec for get every map be expanded (map for radio and map for findtimeitem)

Using O3
about (add up)
* 8  sec for read the file
* 19 sec for a array sorted by user and item
* 29 sec for a array sorted by item 
* 30 sec for calculate ratio
* 41 sec for get every map be expanded (map for radio and map for findtimeitem)

## 5 operation

1. accept
Use user-item-sorted array
Binary search by user ,item and time ( logn)

2. items
Use user-item-sorted array
Binary search by user, and get every  items . (2log n)
beacaseu the item is sorted , we can get every item just by comparsion (2Q)

The max of Q is 14458

3. users 
Use item-sorted array
Binary search by item and time, and get every users. (2Q + 2log n)
Push every users to map (it will remove the depulicated one)
(QlogQ)
and convert map to vector(Q)
The max of Q is 8375857

4. ratio
If the map of vector of  recommand time whose user is accepted the item do not exit (log Q)
than  binary search the item-sorted array(log Q) 
and get every user's(who accept this item) recommand time(find the map) into array and sort it(Q+log(Q)+n log n)
Binary search the recommand time array you in all the user (total)
Binary search the recommand time  who accepted  of this item (accept)
	
The max of Q is 8375857

5. findtime_items
Use item-sorted(the copy one) array
Binary search by item and we will get interval(2log n)
and use the map to record if the interval is sorted by user .(log T )(T small than 6000)
(if not  ,sort it (QlogQ))
The max of Q is 8375857

In the interval
sort for input user (Us log Us) (and remove repulicated)
Binary search by user and we will get all times(US log Q)
Use set to store it and linear output to an array and sort it (Q + QlogQ)

## That's All
thanks 2016/3/21 2:05


###somme tset data and time

normal mode testdata.mix.in
real    3m32.082s
user    3m28.943s
sys     0m1.560s

o3 mode testdata.mix.in
real    0m42.793s
user    0m40.167s
sys     0m1.617s









