//Written by Zac Freeman

//constructs an empty hash table of given size
template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size)
{
    table.resize( prime_below(size) );
    numEntries = 0;
}

//hashtable destructor
template <typename K, typename V>
HashTable<K, V>::~HashTable()
{   makeEmpty(); }

//checks if key k is in the hash table
template <typename K, typename V>
bool HashTable<K, V>::contains(const K & k) const
{
    size_t index = myhash(k);    //index for key

    typename std::list< std::pair<K, V> >::const_iterator itr;

    for (itr = table[index].begin(); itr != table[index].end(); ++itr)
        if ( itr->first == k)
            return true;

    return false;
}

//checks if key-value pair kv is in the hash table
template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> & kv) const
{
    size_t index = myhash(kv.first);    //index for key

    typename std::list< std::pair<K, V> >::const_iterator itr;

    for (itr = table[index].begin(); itr != table[index].end(); ++itr)
        if ( itr->first == kv.first && itr->second == kv.second)
            return true;

    return false;
}

//adds key-value pair kv into hash table
//if kv is already in the hash table, a duplicate is not added
//if the key is in the table with a different value, the value is updated from kv
//returns true if the pair is inserted or the value is updated, and false otherwise
template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv)
{
    size_t index = myhash(kv.first);    //index for key-value pair

    //returns false if there is a duplicate pair
    if ( match(kv) )
        return false;

    //replaces value if there is a duplicate key
    if ( contains(kv.first) )
    {
        typename std::list< std::pair<K, V> >::iterator itr; //iterates over list at index

        for (itr = table[index].begin(); itr != table[index].end(); ++itr)
            if (itr->first == kv.first)
                itr->second = kv.second;
    }
    //inserts the key-value pair at index and returns true if there is no duplicate
    else
	{
        table[index].push_back(kv);

        //rehashes table if average elements per bin is larger than one
        if ( ++numEntries > table.size() )
            rehash();
    }

    return true;
}

//move version of insert()
template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> && kv)
{
    size_t index = myhash(kv.first);    //index for key-value pair

    //returns false if there is a duplicate pair
    if ( match(kv) )
    {
        std::cout << "Error: Duplicate key '" << kv.first
                  << "', duplicate key ignored/n";
        return false;
    }

    //replaces value if there is a duplicate key
    if ( contains(kv.first) )
    {
        typename std::list< std::pair<K, V> >::iterator itr; //iterates over list at index

        for (itr = table[index].begin(); itr != table[index].end(); ++itr)
            if (itr->first == kv.first)
                itr->second = kv.second;
    }
    //inserts the key-value pair at index and returns true if there is no duplicate
    //uses move semantics
    else
    {
        table[index].push_back(std::move(kv));

        //rehashes table if average elements per bin is larger than one
        if ( ++numEntries > table.size() )
            rehash(); 
    }

    return true;
}

//deletes key k and and corresponding value if it is in the hash table
//returns true if k is deleted, and false otherwise
template <typename K, typename V>
bool HashTable<K, V>::remove(const K & k)
{
    size_t index = myhash(k);
    typename std::list< std::pair<K, V> >::iterator itr;

    //searches for element with k in list at index, and erases it if it is found
    for (itr = table[index].begin(); itr != table[index].end(); ++itr)
        if ( itr->first == k)
        {
            table[index].erase(itr);
            --numEntries;
            return true;
        }

    return false;
}

//deletes all elements in the hash table
template<typename K, typename V>
void HashTable<K, V>::clear()
{   makeEmpty(); }

//dispays all entries in the hash table
template<typename K, typename V>
void HashTable<K, V>::dump() const
{
    //iterates through oldTable
    typename std::list< std::pair<K, V> >::const_iterator itr;

    //iterates through each element and prints each to std::cout
    for (int i = 0; i < table.size(); i++)
    {
        //prints first element
        itr = table[i].begin();
        std::cout << "v[" << i << "]: ";

        if ( table[i].begin() == table[i].end() )
        {
            std::cout << std::endl;
            continue;
        }

        std::cout << itr->first << " " << itr->second;
        ++itr;

        //prints any additional entries in list, seperating each with a semicolon
        for (; itr != table[i].end(); ++itr)
            std::cout << ":" << itr->first << " " << itr->second;

        std::cout << '\n';
    }
}

//returns number of entries in hash table
template<typename K, typename V>
size_t HashTable<K, V>::size() const
{   return numEntries; }

//returns number of pair lists in hash table
template<typename K, typename V>
size_t HashTable<K, V>::capacity() const
{   return table.capacity(); }

//loads the content of the file into the hash table
//each line of the file contains one key-value pair, seperated by a white space
//returns false if the file is not found, and true otherwise
template<typename K, typename V>
bool HashTable<K, V>::load(const char *filename)
{
    std::string pair;   //stores key-value pair from line in input file
    K first;            //stores first value in key-value pair
    V second;           //stores second value in key-value pair

    std::stringstream ssPair;       //stores pair to convert to templated types K,V
    std::ifstream input(filename);  //stores input file

    if ( input.is_open() )
    {
        while ( std::getline(input, pair) )
        {
            ssPair << pair << ' ';      //inserts white space seperated pair into stream
            ssPair >> first >> second;  //converts pair values to appropriate types

            insert( std::make_pair(first, second) );
        }

        input.close();
        return true;
    }
    else
        return false;
}

//writes all elements in hash table into file
//each line of file written to contains one key-value pair, seperated by white space
template<typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const
{
    std::ofstream output(filename);
    typename std::list< std::pair<K, V> >::const_iterator itr;

    if ( !output.is_open() )
        return false;

    for (int i = 0; i < table.size(); i++)
        for (itr = table[i].begin(); itr != table[i].end(); ++itr)
            output << itr->first << " " << itr->second << '\n';

    output.close();
    return true;
}

//deletes all elements in hash table
template<typename K, typename V>
void HashTable<K,V>::makeEmpty()
{
    for (int i = 0; i < table.size(); i++)
        table[i].clear();

    numEntries = 0;
}

//rehashes hash table
template<typename K, typename V>
void HashTable<K,V>::rehash()
{
    //shallow copy of hash table
    std::vector< std::list< std::pair<K,V> > > oldTable = table;
    typename std::list< std::pair<K, V> >::iterator itr;    //iterates through oldTable

    //resizes table and clears all elements
    table.clear();
    table.resize( prime_below(2 * oldTable.size()) );
    numEntries = 0;

    //copies data from oldTable to resized table
    for (int i = 0; i < oldTable.size(); i++)
        for (itr = oldTable[i].begin(); itr != oldTable[i].end(); ++itr)
            insert( std::move(*itr) );
}

//returns the index of the vector entry where k should be stored
template<typename K, typename V>
size_t HashTable<K, V>::myhash(const K & k) const
{
    static std::hash<K> hashFunc;
    return ( hashFunc(k) % table.size() );
}

//--Copied from an outside source--
// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n)
{
    if (n > max_prime)
    {
        std::cerr << "** input too large for prime_below()\n";
        return 0;
    }

    if (n == max_prime)
    {
        return max_prime;
    }

    if (n <= 1)
    {
        std::cerr << "** input too small \n";
        return 0;
    }

    // now: 2 <= n < max_prime
    std::vector <unsigned long> v (n+1);
    setPrimes(v);

    while (n > 2)
    {
        if (v[n] == 1)
	        return n;
        --n;
    }

    return 2;
}

//--Copied from an outside source--
//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long> & vprimes)
{
    int i = 0;
    int j = 0;

    vprimes[0] = 0;
    vprimes[1] = 0;
    int n = vprimes.capacity();

    for (i = 2; i < n; ++i)
        vprimes[i] = 1;

    for(i = 2; i*i < n; ++i)
    {
        if (vprimes[i] == 1)
            for(j = i + i; j < n; j += i)
                vprimes[j] = 0;
    }
}
