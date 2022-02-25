#ifndef _PAIR_H
#define _PAIR_H
#include <string>
using namespace std;

int hasherr(int value,int size)
{
    return value%size;
}
int hasherr(string item,int size)
{
    int val = 0;
    for(int i=0;i<item.size();i++)
    {
        val+=item[i];
    }
    return hasherr(val,size);
}

template <class C>
class Pair
{
public:
    C item;
    int code;
    Pair(C i = C(), int c=-1)
        :item(i),code(c)
    {}

    int hasher(int size) const
    {
        return hasherr(item,size);
    }
    bool operator==(const Pair<C> & rhs) const
    {
        return item==rhs.item;
    }
    bool operator!=(const Pair<C> & rhs) const
    {
        return item!=rhs.item;
    }
};


#endif