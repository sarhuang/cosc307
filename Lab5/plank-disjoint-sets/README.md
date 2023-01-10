# Disjoint set implementation

James S. Plank

September, 2020

-----------------------

This contains a header `include/disjoint_set.hpp` and implementation
`src/disjoint_set.cpp` of disjoint sets in C++.   Please read
[http://web.eecs.utk.edu/~jplank/plank/classes/cs302/Notes/Disjoint/](http://web.eecs.utk.edu/~jplank/plank/classes/cs302/Notes/Disjoint/) for an explanation of the data structure and its
implementation.

Everything is in the namespace `plank`.

Read the header file for a brief explanation of methods.  There are extra things in this
implementation that can make your life easier, such as a vector of the set id's, and a 
list of elements in a set.  These are maintained without increasing the running time
complexity of `Union()` and `Find()`

There is a tester program in `bin/dj_test` -- you can use it to explore how Disjoint sets work.
In particular, printing out the disjoint set gives you a lot of information.  It also shows
you how the accessor methods like <b>Get_Links()</b> work.

------------------------------

Here's an example of using `bin/dj_test`

```
UNIX> bin/dj_test 'UNIX>'                       # Run using a prompt of 'UNIX>'
DISJ> I 10                                      # Create a new instance with 10 elements
DISJ> P

Node:    0  1  2  3  4  5  6  7  8  9           # Here is the new instance
Links:  -1 -1 -1 -1 -1 -1 -1 -1 -1 -1
Ranks:   1  1  1  1  1  1  1  1  1  1
Sizes:   1  1  1  1  1  1  1  1  1  1
Set IDs: {0,1,2,3,4,5,6,7,8,9}
Sets: {0}{1}{2}{3}{4}{5}{6}{7}{8}{9}

DISJ> U 0 1                                     # Union(0,1) returns 1
1
DISJ> U 2 3                                     # Union(2,3) returns 3
3
DISJ> U 4 5                                     # Union(4,5) returns 5
5
DISJ> U 1 3                                     # Union(1,3) returns 3
3
DISJ> U 6 7                                     # Union(6,7) returns 7
7
DISJ> U 8 7                                     # Union(8,7) returns 7
7
DISJ> U 9 7                                     # Union(9,7) returns 7
7
DISJ> P

Node:    0  1  2  3  4  5  6  7  8  9           # As you can see, there are three disjoint sets,
Links:   1  3  3 -1  5 -1  7 -1  7  7           # Rooted by 3, 5 and 7 respectively.
Ranks:            3     2     2      
Sizes:            4     2     4      
Set IDs: {3,5,7}
Sets: {0,1,2,3}{4,5}{9,8,6,7}

DISJ> L                                         # This lets you access the links.
 1 3 3 -1 5 -1 7 -1 7 7
DISJ> R                                         # This lets you access the ranks.
SID:  3     Rank:  3                            # They are only valid for roots of the sets
SID:  5     Rank:  2
SID:  7     Rank:  2
DISJ> SZ                                         # This lets you access the set sizes.
SID:  3     Size:  4                             # They are only valid for roots of the sets
SID:  5     Size:  2
SID:  7     Size:  4

DISJ> ID                                         # This gives you a vector of the set ids.
 3 5 7
DISJ> IDI                                        # This is a vector v, where v[set_id[i]] = i.
SID:  3     Index in set_ids():  0               # V[i] is only valid for roots of the sets
SID:  5     Index in set_ids():  1
SID:  7     Index in set_ids():  2

DISJ> E                                          # This gives you lists of elements of sets.
SID:  3: {0,1,2,3}                               # The lists are only non-empty for rootsf the sets
SID:  5: {4,5}
SID:  7: {9,8,6,7}
DISJ> Q
UNIX> 
```
