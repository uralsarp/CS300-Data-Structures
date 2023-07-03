#ifndef _HASHTABLE_H
#define _HASHTABLE_H
#include <iostream>
#include <vector>

using namespace std;

template <class HashedObj>
        class HashTable
        {
		private:
			struct HashEntry
			{
				HashedObj element; //element that we want to store in a node of the hash table
				int num; //the number that we assign to each longest prefix
			};
            
			vector<HashEntry> arr; //hash table
			const HashedObj ITEM_NOT_FOUND; //sentinel value
			int findPos(const HashedObj & x) const; //find the position of the element in the table
		
          public:
            HashTable<HashedObj>(const HashedObj & notFound, int size = 4096) //constructor
				: ITEM_NOT_FOUND(notFound), arr(size){
				for (int i=0; i<arr.size(); i++){
					arr[i].element = ITEM_NOT_FOUND; //assign all values initially to the sentinel value
				}
			}
            int find( const HashedObj & x ) const;
            void insert( const HashedObj & x, const int idx);

		};

#include "HashTable.cpp"
#endif