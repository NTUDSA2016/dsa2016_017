#HW2
##my data struct

A big array ( vrowdata ) for store the original data
Two sorted array , one sorted by user , one sorted by items

It will cause about 4n + 2nlogn (stable sort)

Beacuse the original data is sorted by time , so my two array will sorted by time too.

On the workstation

* 43.231873 for read the file
* 110.082512 for a array sorted by user
* 202.039383 for a array sorted by item 

## 5 operation

1. accept
Use user-sorted array
Binary search by user and time , and linear search for item . (log n + Q)
The max of Q will cause 120  times to find

2. items
Use user-sorted array
Binary search by user, and get every  items . (2log n)
Use set to store items and an array to store both items 
Every time I find the same items than I will  delete the item in set for not repeatily getting same items
( 2QlogQ ) 
( Beacuse set can delete depulicated item and easy to find the item)
and sort the array 
( QlogQ ) 
The max of Q is 14458

3. users 
Use item-sorted array
Binary search by item and time, and get every items . (2log n)
the same operation as items
(3QlogQ)
The max of Q is 8375857

4. ratio
Use item-sorted array
Binary search by item , and get every user. (log n)
Use map to store user and recommand times and a set to store accepted time (2QlogQ)
and linear search the user over the thershold . If it does , we find it in the set . THus we will get radio  (Q+QlogQ)
The max of Q is 8375857

5. findtime_items
Use item-sorted array
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







