#ifndef __UTIL_DEQUE__
#define __UTIL_DEQUE__

#include <define.h>

#include <memory>
#include <functional>

#include <util/os/Log.h>

namespace simple3deditor {

template <typename T>
class Deque : public Object {
private:
    static constexpr uint CHUNK_SIZE_BIT = 6;
    static constexpr uint CHUNK_SIZE = (1 << CHUNK_SIZE_BIT);
    static constexpr uint CHUNK_SIZE_MASK = CHUNK_SIZE - 1;
    static constexpr uint CHUNK_FIRST_INDEX = CHUNK_SIZE >> 1;

    T** mChunks;
    size_t mSize;
    size_t mNbChunks;
    size_t mFirstChunkIndex;
    size_t mLastChunkIndex;
    byte mFirstItemIndex;
    byte mLastItemIndex;

    T& operator[](size_t idx){
        return GetItem(idx);
    }

    T& GetItem(size_t virtualIndex){
        if (virtualIndex >= mSize){
            DebugError("Critical: Deque<T>::Insert(%llu) When Index Overflow", virtualIndex);
            return;
        }

        size_t chunkIndex = mFirstChunkIndex;
        size_t itemIndex = mFirstItemIndex;

        const size_t nbItemsFirstChunk = CHUNK_SIZE - mFirstItemIndex;

        if (virtualIndex < nbItemsFirstChunk) {
            itemIndex += virtualIndex;
        }else{
            virtualIndex -= nbItemsFirstChunk;
            chunkIndex++;

            chunkIndex += virtualIndex >> CHUNK_SIZE_BIT;
            itemIndex = virtualIndex & CHUNK_SIZE_MASK;
        }

        return mChunks[chunkIndex][itemIndex];
    }

    void Reserve(size_t atLeastNbChunks = 0) {
        if (atLeastNbChunks > 0 && atLeastNbChunks <= mNbChunks) {
            return;
        }

        size_t newNbChunks = mNbChunks == 0 ? 3 : 2 * mNbChunks - 1;
        if (atLeastNbChunks > 0 && newNbChunks < atLeastNbChunks) {
            newNbChunks = atLeastNbChunks | 1;
        }
        const size_t halfNbChunksToAdd = mNbChunks == 0 ? 1 : ((mNbChunks - 1) >> 1);

        T** newChunks = new T*[newNbChunks];
        if (mNbChunks > 0) {
            memcpy(newChunks + halfNbChunksToAdd, mChunks, mNbChunks * sizeof(T*));
            delete[] mChunks;
        }

        mChunks = newChunks;

        // 申请中间区块
        if (mNbChunks == 0) 
            mChunks[newNbChunks >> 1] = new T[CHUNK_SIZE];
        mNbChunks = newNbChunks;

        // 申请前后半部分区块
        for (size_t i = 0; i < halfNbChunksToAdd; i++) {
            mChunks[i] = new T[CHUNK_SIZE];
            mChunks[mNbChunks - 1 - i] = new T[CHUNK_SIZE];
        }

        mFirstChunkIndex += halfNbChunksToAdd;
        mLastChunkIndex += halfNbChunksToAdd;
    }

public:
    class Iterator {
    private:
        size_t mVirtualIndex;
        const Deque<T>* mDeque;

    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using const_pointer = T const*;
        using reference = T&;
        using const_reference = const T&;
        using iterator_category = std::random_access_iterator_tag;

        Iterator() = default;
        Iterator(const Deque<T>* deque, size_t virtualIndex) : mVirtualIndex(virtualIndex), mDeque(deque) {}
        Iterator(const Iterator& it) : mVirtualIndex(it.mVirtualIndex), mDeque(it.mDeque) {}

        reference operator*() {
            return mDeque->GetItem(mVirtualIndex);
        }

        const_reference operator*() const {
            return mDeque->GetItem(mVirtualIndex);
        }

        const_pointer operator->() const {
            return &(mDeque->GetItem(mVirtualIndex));
        }

        Iterator& operator++() {
            mVirtualIndex++;
            return *this;
        }

        Iterator operator++(int number) {
            Iterator tmp = *this;
            mVirtualIndex++;
            return tmp;
        }

        Iterator& operator--() {
            mVirtualIndex--;
            return *this;
        }

        Iterator operator--(int number) {
            Iterator tmp = *this;
            mVirtualIndex--;
            return tmp;
        }

        Iterator operator+(const difference_type& n) {
            return Iterator(mDeque, mVirtualIndex + n);
        }

        Iterator& operator+=(const difference_type& n) {
            mVirtualIndex += n;
            return *this;
        }

        Iterator operator-(const difference_type& n) {
            return Iterator(mDeque, mVirtualIndex - n);
        }

        Iterator& operator-=(const difference_type& n) {
            mVirtualIndex -= n;
            return *this;
        }

        difference_type operator-(const Iterator& iterator) const {
            return mVirtualIndex - iterator.mVirtualIndex;
        }

        bool operator<(const Iterator& other) const {
            return mVirtualIndex < other.mVirtualIndex;
        }

        bool operator>(const Iterator& other) const {
            return mVirtualIndex > other.mVirtualIndex;
        }

        bool operator<=(const Iterator& other) const {
            return mVirtualIndex <= other.mVirtualIndex;
        }

        bool operator>=(const Iterator& other) const {
            return mVirtualIndex >= other.mVirtualIndex;
        }

        bool operator==(const Iterator& iterator) const {
            return mVirtualIndex == iterator.mVirtualIndex;
        }

        bool operator!=(const Iterator& iterator) const {
            return !(*this == iterator);
        }

        friend class Deque;
    };

    Deque(MemoryAllocator& allocator)
        : mChunks(NULL), mSize(0), mNbChunks(0), mFirstChunkIndex(1),
            mLastChunkIndex(1), mFirstItemIndex(CHUNK_FIRST_INDEX),
            mLastItemIndex(CHUNK_FIRST_INDEX) {
        Reserve();
    }

    /// Copy constructor
    Deque(const Deque<T>& deque)
        : mSize(0), mNbChunks(0), mFirstChunkIndex(1),
            mLastChunkIndex(1), mFirstItemIndex(CHUNK_FIRST_INDEX),
            mLastItemIndex(CHUNK_FIRST_INDEX) {
        Reserve(deque.mNbChunks);

        if (deque.mSize > 0) {
            const size_t dequeHalfSize1 = (deque.mSize + 1) >> 1;
            const size_t dequeHalfSize2 = deque.mSize - dequeHalfSize1;

            for(size_t i = 0; i < dequeHalfSize1; i++)
                AddFront(deque[dequeHalfSize1 - 1 - i]);
            for(size_t i = 0; i < dequeHalfSize2; i++)
                AddBack(deque[dequeHalfSize1 + i]);
        }
    }

    ~Deque() {
        Clear();
        for (size_t i=0; i < mNbChunks; i++)
            delete[] mChunks[i];
        delete[] mChunks
    }

    void AddBack(const T& element) {
        if (mLastItemIndex == CHUNK_SIZE - 1) {
            if (mLastChunkIndex == mNbChunks - 1)
                Reserve();

            mLastItemIndex = 0;
            mLastChunkIndex++;
        }
        else if (mSize != 0) {
            mLastItemIndex++;
        }

        new (static_cast<void*>(&(mChunks[mLastChunkIndex][mLastItemIndex]))) T(element);

        mSize++;

        assert(mFirstChunkIndex >= 0 && mLastChunkIndex < mNbChunks);
        assert(mFirstItemIndex >= 0 && mFirstItemIndex < CHUNK_SIZE);
        assert(mLastItemIndex >= 0 && mLastItemIndex < CHUNK_SIZE);
        assert(mFirstChunkIndex <= mLastChunkIndex);
    }

    void AddFront(const T& element) {
        if (mFirstItemIndex == 0) {
            if (mFirstChunkIndex == 0) {
                Reserve();
            }

            mFirstItemIndex = CHUNK_SIZE - 1;
            mFirstChunkIndex--;
        }
        else if (mSize != 0) {
            mFirstItemIndex--;
        }

        new (static_cast<void*>(&(mChunks[mFirstChunkIndex][mFirstItemIndex]))) T(element);

        mSize++;

        #ifdef DEBUG
        assert(mFirstChunkIndex >= 0 && mLastChunkIndex < mNbChunks);
        assert(mFirstItemIndex >= 0 && mFirstItemIndex < CHUNK_SIZE);
        assert(mLastItemIndex >= 0 && mLastItemIndex < CHUNK_SIZE);
        assert(mFirstChunkIndex <= mLastChunkIndex);
        #endif
    }

    void PopFront() {
        if (mSize > 0) {
            mChunks[mFirstChunkIndex][mFirstItemIndex].~T();

            mSize--;

            if (mSize == 0) {
                mFirstChunkIndex = mNbChunks / 2;
                mFirstItemIndex = CHUNK_FIRST_INDEX;
                mLastChunkIndex = mFirstChunkIndex;
                mLastItemIndex = CHUNK_FIRST_INDEX;
            }
            else if (mFirstItemIndex == CHUNK_SIZE - 1){
                mFirstChunkIndex++;
                mFirstItemIndex = 0;
            }
            else {
                mFirstItemIndex++;
            }

            #ifdef DEBUG
            assert(mFirstChunkIndex >= 0 && mLastChunkIndex < mNbChunks);
            assert(mFirstItemIndex >= 0 && mFirstItemIndex < CHUNK_SIZE);
            assert(mLastItemIndex >= 0 && mLastItemIndex < CHUNK_SIZE);
            assert(mFirstChunkIndex <= mLastChunkIndex);
            #endif
        }
    }

    void PopBack() {
        if (mSize > 0) {
            mChunks[mLastChunkIndex][mLastItemIndex].~T();

            mSize--;

            if (mSize == 0) {
                mFirstChunkIndex = mNbChunks / 2;
                mFirstItemIndex = CHUNK_FIRST_INDEX;
                mLastChunkIndex = mFirstChunkIndex;
                mLastItemIndex = CHUNK_FIRST_INDEX;
            }
            else if (mLastItemIndex == 0){
                mLastChunkIndex--;
                mLastItemIndex = CHUNK_SIZE - 1;
            }
            else {
                mLastItemIndex--;
            }

            #ifdef DEBUG
            assert(mFirstChunkIndex >= 0 && mLastChunkIndex < mNbChunks);
            assert(mFirstItemIndex >= 0 && mFirstItemIndex < CHUNK_SIZE);
            assert(mLastItemIndex >= 0 && mLastItemIndex < CHUNK_SIZE);
            assert(mFirstChunkIndex <= mLastChunkIndex);
            #endif
        }
    }

    const T& GetFront() const {
        if (mSize == 0){
            DebugError("Critical: Deque<T>::GetFront When Size Is 0");
            return **mChunks;
        }
        return mChunks[mFirstChunkIndex][mFirstItemIndex];
    }

    const T& GetBack() const {
        if (mSize == 0){
            DebugError("Critical: Deque<T>::GetBack When Size Is 0");
            return **mChunks;
        }
        return mChunks[mLastChunkIndex][mLastItemIndex];
    }

    void Clear() {
        if (mSize > 0) {
            for (size_t i = 0; i < mSize; i++) {
                GetItem(i).~T();
            }

            mSize = 0;

            mFirstChunkIndex = mNbChunks >> 1;
            mLastChunkIndex = mFirstChunkIndex;
            mFirstItemIndex = CHUNK_FIRST_INDEX;
            mLastItemIndex = CHUNK_FIRST_INDEX;
        }
    }

    size_t Size() const {
        return mSize;
    }

    bool Empty() const {
        return mSize == 0;
    }

    T& operator[](const uint index) {
        return GetItem(index);
    }

    const T& operator[](const uint index) const {
        return GetItem(index);
    }

    bool operator==(const Deque<T>& deque) const {
        if (mSize != deque.mSize) return false;

        for (size_t i = 0; i < mSize; i++) {
            if (GetItem(i) != deque.GetItem(i)) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const Deque<T>& deque) const {
        return !((*this) == deque);
    }

    Deque<T>& operator=(const Deque<T>& deque) {
        if (this != &deque) {
            Clear();

            if (deque.mSize > 0) {
                const size_t nbUsedChunks = deque.mLastChunkIndex - deque.mFirstChunkIndex + 1;

                Reserve(nbUsedChunks);

                const size_t dequeHalfSize1 = (deque.mSize + 1) >> 1;
                const size_t dequeHalfSize2 = deque.mSize - dequeHalfSize1;

                for(size_t i=0; i < dequeHalfSize1; i++)
                    AddFront(deque[dequeHalfSize1 - 1 - i]);
                for(size_t i=0; i < dequeHalfSize2; i++)
                    AddBack(deque[dequeHalfSize1 + i]);
            }
        }

        return *this;
    }

    Iterator Begin() const {
        return Iterator(this, 0);
    }

    Iterator End() const {
        return Iterator(this, mSize);
    }

    Deque<T>& Foreach(std::function<void(T)> f){
        size_t nbItemsFirstChunk = CHUNK_SIZE - mFirstItemIndex;
        const size_t nbItemsRem =  mSize - nbItemsFirstChunk;

        for(size_t i = 0; i < nbItemsFirstChunk; i++)
            f(mChunks[mFirstChunkIndex][mFirstItemIndex + i]);
        mFirstChunkIndex++;
        for(size_t i = 0; i < nbItemsRem; i++)
            f(mChunks[mFirstChunkIndex + (i >> CHUNK_SIZE_BIT)][i & CHUNK_SIZE_MASK]);

        return *this;
    }
};

template <typename T>
void Free(Deque<T*>& deque){
    deque.Foreach([](T* item){
        delete item;
    });
}

}

#endif
