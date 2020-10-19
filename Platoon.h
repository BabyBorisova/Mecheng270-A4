#ifndef _Platoon_H_
#define _Platoon_H_

#include <vector>
#include <string>
#include <sstream>
#include "Car.h"

using namespace std;

class Platoon
{
    private:
        Car* head = NULL;
        Car* tail = NULL;

    public:
        Platoon(string init);
        ~Platoon();

        Car* get_tail();
        Car* get_head();

        void remove(Car* c); 
        void append(Car* c); 
        void prepend(Car* c); 
        void insert(Car* c);

        const bool pos_is_empty(int pos);
        

};
#endif
