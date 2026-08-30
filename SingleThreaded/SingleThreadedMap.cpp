#include "SingleThreadedMap.hpp"



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

    for ( std::size_t i = 0; i < mCapacity; i++ ) {

        Node* oldBucketHead = other.mData[i];
        Node* newBucketHead = mData[i];

        //walk and delete the linked list at each level

        while (oldBucketHead != nullptr) {
            newBucketHead = new Node(oldBucketHead);

            oldBucketHead = oldBucketHead->next;
            newBucketHead = newBucketHead->next;
        }
    }

    mSize = other.mSize;
    mCapacity = other.mCapacity;
}   

HashMap::HashMap(const HashMap& other) {
    Node** mData = new Node*[other.mCapacity];

    if ( mData == nullptr ) {
        return;
    }

    copyData(other);

}

HashMap& HashMap::operator=(const HashMap& other) {
    if ( this != &other ) {
        Node** mData = new Node*[other.mCapacity];

        if ( mData == nullptr ) {
            return;
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