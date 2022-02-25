#ifndef _HASHTABLE_H
#define _HASHTABLE_H
#include <vector>
#include "Pair.h"
using namespace std;

template <class C>
class HashTable
{
private:
    
    int size;
    int capacity;
public:
    vector<Pair<C> > v;
    HashTable()
    {
        v = vector<Pair<C> >(4096);
        size = 0;
        capacity=4096;
    }

    void initialize(vector<Pair<C> > initialvalues)
    {
        for(int i=0;i<initialvalues.size();i++)
        {
            insert(initialvalues[i]);
        }
    }
    void insert(const Pair<C> & pair)
    {
        int hashval = pair.hasher(capacity);
        int count = 0;
        Pair<C> blank;
        while(count<capacity)
        {
            int position = (hashval+count)%capacity;
            if(position<0)
                position+=capacity;
            if(v[position]==blank)
            {
                v[position] = pair;
                size++;
                if(size==capacity)
                    rehash();
                return;
            }
            count++;
        }
    }
    int find(const C & key) const
    {
        int hashval = hasherr(key,capacity);
        int count = 0;
        Pair<C> blank;
        while(count<capacity)
        {
            int position = (hashval+count)%capacity;
            if(position<0)
                position+=capacity;
            if(v[position].item == key)
            {
                return v[position].code;
            }
            else if(v[position]==blank)
            {
                return -1;
            }
            count++;
        }
        return -1;
    }
    void rehash()
    {
        vector<Pair<C> > oldpairs = v;
        v = vector<Pair<C> >(2*capacity);
        capacity = 2*capacity;
        for(int i=0;i<oldpairs.size();i++)
        {
            if(oldpairs[i]!=Pair<C>())
                insert(oldpairs[i]);
        }
    }
};
#endif