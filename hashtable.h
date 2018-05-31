//Written by Zac Freeman

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <fstream>
#include <sstream>

namespace cop4530
{

    static const unsigned int max_prime = 1301081;
    static const unsigned int default_capacity = 11;

    template<typename K, typename V>
    class HashTable
    {

        public:
            HashTable(size_t size = 101);   //constructor
            ~HashTable();                   //destructor

            bool contains(const K & k) const;               //checks if k is in table
            bool match(const std::pair<K, V> & kv) const;   //checks if pair is in table

            bool insert(const std::pair<K, V> & kv);    //inserts pair into table
            bool insert(std::pair<K, V> && kv);         //inserts pair using move semantics
            bool remove(const K & k);                   //removes k from table
            void clear();                               //deletes all elements in hash table
            void dump() const;                          //display all entries in table
            size_t size() const;                        //returns number of entries in table
            size_t capacity() const;                    //returns number of pair lists in table

            bool load(const char *filename);                //load contents of file into table
            bool write_to_file(const char *filename) const; //write entries in table to file


        private:
            //empty hash table
            std::vector< std::list< std::pair<K, V> > > table;

            size_t numEntries;    //number of entries in hash table

            void makeEmpty();                   //delete all elements in table
            void rehash();                      //rehashes table
            size_t myhash(const K & k) const;   //return index where k should be stored

            unsigned long prime_below (unsigned long n);
            void setPrimes(std::vector<unsigned long>& v);
    };

    #include "hashtable.hpp"
}

#endif
