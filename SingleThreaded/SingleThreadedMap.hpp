#include <string>
#include <cstddef>
#include <utility>

/**
 * Implement a single threaded HashMap using (string, int) as (key, value). 
 * Please use separate chaining over open addressing as to not invalidate
 * references to elements as the test suite will test against it.
 * 
 * Feel free to implement as you see fit -- have fun!
 */

class HashMap {

/**
 * Declare any private variables you need here.
 * 
 * NOTE: Use your own hash function for std::string instead of std::hash 
 * Be creative :)
 */

private:




/**
 * These are the APIs you need to support. The test suite will test against
 * these.
 * 
 * Good Luck!! :)
 */

public:

    //default constructor
    HashMap();

    //dont forget to destroy me
    ~HashMap();

    //copy constructor
    HashMap(const HashMap& other);

    //copy assigment operataor
    HashMap& operator=(const HashMap& other);

    //move constructor
    HashMap(HashMap&& other);

    //move assigment operataor
    HashMap& operator=(HashMap&& other);

    // .first  = true if newly inserted, false if key already existed
    // .second = pointer to the value stored at that key (new or existing)
    //
    //NOTE: insert will not overwrite if a value exists with the corresponding
    //      key
    std::pair<bool, int*> insert(const std::string& key, int value);

    // returns number of elements removed (0 or 1)
    std::size_t erase(const std::string& key);

    // check if it exists in the map
    bool contains(const std::string& key) const;

    // returns nullptr if key not found, otherwise pointer to the stored value
    int* find(const std::string& key);

    // map operator[] access — inserts default value if missing
    int& operator[](const std::string& key);

    //other basic functions
    std::size_t size() const;
    bool empty() const;
    void clear();

    std::size_t bucket_count() const;
    double load_factor() const;
};