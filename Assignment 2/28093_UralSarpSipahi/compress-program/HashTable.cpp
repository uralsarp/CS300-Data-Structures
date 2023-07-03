#include "HashTable.h"

using namespace std;


template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj & x) const{
            
	hash<HashedObj> hasher;
	unsigned int p = hasher(x); // p = current position
	p = p % arr.size(); //modulo

	while(arr[p].element != ITEM_NOT_FOUND && arr[p].element != x){ //if there is a collision

		p++;  // linear probing

        if(p >= arr.size()){             // perform the mod
			p = p-arr.size();                // if necessary
		}
	}
    return p;
}

template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x, const int num){ 
            // Insert x as active
    int currentPos = findPos(x);

    if(arr[currentPos].element==x){ //if item already exists in the list
		return;
	}
    arr[currentPos].element = x;
	arr[currentPos].num=num;

}

template <class HashedObj>
int HashTable<HashedObj>::find(const HashedObj & x) const{
	int currentPos = findPos(x);

	if (arr[currentPos].element==x) 
		return arr[currentPos].num; //if item is in the table return its corresponding number
 
	return -1; //if item is not found
}

