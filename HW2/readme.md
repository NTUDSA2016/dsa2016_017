#HW2
##my data struct

A big array ( vrowdata ) for store the original data
Two sorted array , one sorted by user and item , one sorted by items
A copy array from item-sort

A map for calculate the time of users recommad by system
A array for sort recommand time 

A map for store weather the item-sort array is sorted by user
A map for store the vector of how many people accept the item

It will cause about n + nlogn (stable sort)

Beacuse the original data is sorted by time , so my two array will sorted by time too.

On the workstation (2:00 linux9)

about
* 17 sec for read the file
* 102 sec for a array sorted by user and item
* 195 sec for a array sorted by item 
* 203 sec for calculate ratio

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
Binary search by item and time, and get every items . (2log n)
the same operation as items
(3QlogQ)
The max of Q is 8375857

4. ratio

Binary search the threshold in array

if the map doesn't have this item
than  binary search the item-sorted array and get every this
store the accept user in an array
ask the map if the accept user is over theshold
	
The max of Q is 8375857

5. findtime_items

Use item-sorted(the copy one) array
Binary search by item and we will get interval(2log n)
and use the map to record if the interval is sorted by user .(log T )(T small than 6000)
(if not  ,sort it (QlogQ))
(Use stable sort so the time is ordered)
The max of Q is 8375857

In the interval
Binary search by user and we will get all times(US log Q)
Use set to store it and linear output to an array (Q + QlogQ)

## That's All
thanks 2016/3/21 2:05







