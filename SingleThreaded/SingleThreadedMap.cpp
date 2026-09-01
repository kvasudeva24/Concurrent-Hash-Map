#include "SingleThreadedMap.hpp"
#include <iostream>



HashMap::HashMap() = default;

void HashMap::destroy() {

    for ( std::size_t i = 0; i < mCapacity; i++ ) {

        Node* bucketHead = mData[i];

        //walk and delete the linked list at each level

        Node* next = nullptr;

        while (bucketHead != nullptr) {
            next = bucketHead->next;
            delete bucketHead;

            bucketHead = next;
        }

    }

    mData = nullptr;
    mSize = 0;
    mCapacity = 0;

}

HashMap::~HashMap() {
    destroy();
}

//the caller is responsible for allocating memory when we need to copy data
void HashMap::copyData(const HashMap& other) {

    for ( std::size_t i = 0; i < other.mCapacity; i++ ) {

        Node* oldBucketHead = other.mData[i];
        Node* newBucketHead = mData[i];
        Node* prev = nullptr;

        //walk and delete the linked list at each level

        while (oldBucketHead != nullptr) {
            newBucketHead = new Node(oldBucketHead);
            if (prev == nullptr) { mData[i] = newBucketHead; }

            oldBucketHead = oldBucketHead->next;

            if ( prev ) { prev->next = newBucketHead; prev = newBucketHead; }
            newBucketHead = newBucketHead->next;
        }
    }

    mSize = other.mSize;
    mCapacity = other.mCapacity;
}   

HashMap::HashMap(const HashMap& other) {
    mData = new Node*[other.mCapacity]{};

    if ( mData == nullptr ) {
        return;
    }

    copyData(other);

}

HashMap& HashMap::operator=(const HashMap& other) {
    if ( this != &other ) {
        Node** mData = new Node*[other.mCapacity]{};

        if ( mData == nullptr ) {
            return *this;
        }

        copyData(other);
    }

    return *this;
}

HashMap::HashMap(HashMap&& other) {
    mData = std::exchange(other.mData, nullptr);
    mSize = std::exchange(other.mSize, 0);
    mCapacity = std::exchange(other.mCapacity, 0);
}

HashMap& HashMap::operator=(HashMap&& other) {
    if ( this != &other ) {
        mData = std::exchange(other.mData, nullptr);
        mSize = std::exchange(other.mSize, 0);
        mCapacity = std::exchange(other.mCapacity, 0);
    }
    return *this;
}

void HashMap::resize() {
    std::size_t newCapacity = (mCapacity == 0) ? 5 : mCapacity * 3;

    Node** newData = new Node*[newCapacity]{};

    if ( newData == nullptr ) {
        return;
    } 


    std::size_t newBuckets = 0;
    std::size_t newSize = mSize;

    for ( std::size_t i = 0; i < mCapacity; i++ ) {

        Node* bucket = mData[i];

        if ( bucket == nullptr ) {
            continue;
        }

        while ( bucket != nullptr ) {

            std::size_t newHashIndex = hashFunction(bucket->key) % newCapacity;
            Node* newBucket = newData[newHashIndex];
            if ( newBucket == nullptr ) { newBuckets++; }

            Node* newNode = new Node(bucket);

            newNode->next = newBucket;
            newData[newHashIndex] = newNode;

            bucket = bucket->next;
        }
    }

    destroy();
    mData = newData;
    mSize = newSize;
    mCapacity = newCapacity;
    mBuckets = newBuckets;

}

std::pair<bool, int*> HashMap::insert(const std::string& key, int value) {

    if (mCapacity == 0 || load_factor() > 0.7) {
        resize();
    }

    std::size_t hashIndex = hashFunction(key) % mCapacity;
    Node* bucket = mData[hashIndex];
    Node* bucketIter = bucket;

    if (bucketIter == nullptr) {
        mBuckets++;
    }

    bool found = false;
    int* valuePtr = nullptr;

    while ( bucketIter != nullptr ) {
        if ( bucketIter->key == key ) {
            found = true;
            valuePtr = &bucketIter->value;
            return {!found, valuePtr};
        }

        bucketIter = bucketIter->next;
    }

    //we have not found the key so we must insert
    Node* newNode = new Node(key, value);
    valuePtr = &newNode->value;

    if ( newNode == nullptr ) {
        return {!found, valuePtr};
    }

    newNode->next = bucket;
    mData[hashIndex] = newNode;

    mSize++;

    return {!found, valuePtr};


}

std::size_t HashMap::erase(const std::string& key) {

    if ( contains(key) == false ) {
        return 0;
    }

    std::size_t hashIndex = hashFunction(key) % mCapacity;

    Node* bucket = mData[hashIndex];
    Node* bucketIter = bucket;
    Node* prev = nullptr;

    while ( bucketIter != nullptr ) {
        if ( bucketIter->key == key ) {
            
            if ( prev == nullptr ) {
                mData[hashIndex] = bucketIter->next;

            } else {
                prev->next = bucketIter->next;
            }

            delete bucketIter;
            break;
        }

        bucketIter = bucketIter->next;
    }

    return 1;
}

int* HashMap::find(const std::string& key) const {
    std::size_t hashIndex = hashFunction(key) % mCapacity;
    Node* bucketIter = mData[hashIndex];

    int* valuePtr = nullptr;

    while ( bucketIter != nullptr ) {
        if ( bucketIter->key == key ) {
            valuePtr = &bucketIter->value;
            return valuePtr;
        }

        bucketIter = bucketIter->next;
    }

    return valuePtr;
}

bool HashMap::contains(const std::string& key) const {
    return (find(key) != nullptr);
}

int& HashMap::operator[](const std::string& key) {
    return (*(insert(key, int{}).second));
}

std::size_t HashMap::size() const {
    return mSize;
}

bool HashMap::empty() const {
    return (mSize == 0);
}

void HashMap::clear() {
    destroy();
}

std::size_t HashMap::bucket_count() const {
    return mBuckets;
}

double HashMap::load_factor() const {
    return mSize/(static_cast<double>(mCapacity));
}