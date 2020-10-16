#ifndef _Platoon_H_
#define _Platoon_H_

#include <vector>
#include <string>
#include <sstream>
#include "Car.h"

using namespace std;
/*
Basically a linked list or Cars. 
This list is ordered by being the cars in a lane of the road, 
from left to right.
Cars have next, prev (doubly linked).
Head is the first car, tail is the last
They may be the same car (if only one in the lane)
if head/tail == NULL, no cars in lane
*/
class Platoon
{
    private:
        Car* head = NULL;
        Car* tail = NULL;

    public:
        Platoon(string init);
        Car* get_tail();
        Car* get_head();

        /* 
        you may create methods below
        For example, something like:

        void remove(Car* c); 
        void append(Car* c); 
        void prepend(Car* c); 
        void insert(Car* c);
         */
        

};
#endif
