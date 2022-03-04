#ifndef __UTILS_QUEUE__
#define __UTILS_QUEUE__

#include <define.h>

#include <utils/os/Log.h>

template <typename T>
class Queue : public Object {
private:
    T* data;
    // Size 是实际大小减一，以便取与
    size_t size;
    size_t head;
    size_t tail;
    size_t dataSize;

    void Check(size_t reserve){
        size_t len = dataSize + reserve;
        if (len <= size)
            return;
        head &= size;
        tail &= size;
        size_t newSize = (++size);
        while (len >= newSize)
            newSize <<= 1;
        T* newData = new T[newSize];
        if (head <= tail){
            for (size_t i = head; i < tail; i++)
                newData[i] = data[i];
        }else{
            for (size_t i = head, tmp = newSize - size; i < size; i++)
                newData[i + tmp] = data[i];
            head += (newSize - size);
            for (size_t i = 0; i < tail; i++)
                newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = newSize - 1;
    }

public:
    Queue(){
        size = 7;
        data = new T[size + 1];
        head = 0;
        tail = 0;
        dataSize = 0;
    }

    Queue(Queue&& queue){
        size = queue.size;
        data = new T[size + 1];
        head = (queue.head & size);
        tail = (queue.tail & size);
        dataSize = queue.dataSize;
        if (head <= tail){
            for (size_t i = head; i < tail; i++)
                data[i] = queue.data[i];
        }else{
            for (size_t i = head; i <= size; i++)
                data[i] = queue.data[i];
            for (size_t i = 0; i < tail; i++)
                data[i] = queue.data[i];
        }
    }

    Queue(const Queue& queue){
        size = queue.size;
        data = new T[size + 1];
        head = (queue.head & size);
        tail = (queue.tail & size);
        dataSize = queue.dataSize;
        if (head <= tail){
            for (size_t i = head; i < tail; i++)
                data[i] = queue.data[i];
        }else{
            for (size_t i = head; i <= size; i++)
                data[i] = queue.data[i];
            for (size_t i = 0; i < tail; i++)
                data[i] = queue.data[i];
        }
    }

    Queue& operator=(Queue&& queue){
        delete[] data;
        size = queue.size;
        data = new T[size + 1];
        head = (queue.head & size);
        tail = (queue.tail & size);
        dataSize = queue.dataSize;
        if (head <= tail){
            for (size_t i = head; i < tail; i++)
                data[i] = queue.data[i];
        }else{
            for (size_t i = head; i <= size; i++)
                data[i] = queue.data[i];
            for (size_t i = 0; i < tail; i++)
                data[i] = queue.data[i];
        }
        return *this;
    }

    Queue& operator=(const Queue& list){
        delete[] data;
        size = queue.size;
        data = new T[size + 1];
        head = (queue.head & size);
        tail = (queue.tail & size);
        dataSize = queue.dataSize;
        if (head <= tail){
            for (size_t i = head; i < tail; i++)
                data[i] = queue.data[i];
        }else{
            for (size_t i = head; i <= size; i++)
                data[i] = queue.data[i];
            for (size_t i = 0; i < tail; i++)
                data[i] = queue.data[i];
        }
        return *this;
    }

    ~Queue(){
        if (data == NULL){
            DebugError("Critical: Queue<T>::~Queue Data Pointer Is NULL");
        }
        delete[] data;
    }

    size_t Size(){
        return dataSize;
    }

    bool Empty(){
        return dataSize == 0;
    }

    Queue<T>& Append(T val){
        Check(5);
        data[(tail++) & size] = val;
        dataSize++;
        return *this;
    }

    Queue<T>& AddFront(T val){
        Check(5);
        data[(--head) & size] = val;
        dataSize++;
        return *this;
    }

    T RemoveFront(){
        if (dataSize == 0){
            DebugError("Critical: Queue<T>::RemoveFront When Size Is 0");
            return data[head];
        }
        dataSize--;
        return data[(head++) & size];
    }

    T RemoveBack(){
        if (dataSize == 0){
            DebugError("Critical: Queue<T>::RemoveBack When Size Is 0");
            return data[head];
        }
        dataSize--;
        return data[(--tail) & size];
    }

    T& GetFront(){
        if (dataSize == 0){
            DebugError("Critical: Queue<T>::GetFront When Size Is 0");
        }
        return data[head];
    }

    T& GetBack(){
        if (dataSize == 0){
            DebugError("Critical: Queue<T>::GetBack When Size Is 0");
            return data[head];
        }
        return data[(tail - 1) & size];
    }

    T& operator[](size_t index){
        if (index >= dataSize){
            DebugError("Critical: Queue<T>::operator[](%llu) When Index Overflow", index);
        }
        return data[(head + index) & size];
    }

    T& GetItem(size_t index){
        if (index >= dataSize){
            DebugError("Critical: Queue<T>::GetItem(%llu) When Index Overflow", index);
        }
        return data[(head + index) & size];
    }

    void Clear(){
        head = 0;
        tail = 0;
        dataSize = 0;
    }

    void Clear(size_t memory){
        delete[] data;
        size = 1;
        while (memory >= size)
            size <<= 1;
        data = new T[size--];
        head = 0;
        tail = 0;
        dataSize = 0;
    }

    Queue<T>& Foreach(void(*func)(T)){
        size_t i;

        head &= size;
        tail &= size;
        if (head <= tail){
            for (i = head; i < tail; i++)
                func(data[i]);
        }else{
            for (i = head; i <= size; i++)
                func(data[i]);
            for (i = 0; i < tail; i++)
                func(data[i]);
        }
    }

    template <typename Tp>
    Queue<T>& Foreach(void(*func)(T, Tp), Tp user){
        size_t i;
        
        head &= size;
        tail &= size;
        if (head <= tail){
            for (i = head; i < tail; i++)
                func(data[i], user);
        }else{
            for (i = head; i <= size; i++)
                func(data[i], user);
            for (i = 0; i < tail; i++)
                func(data[i], user);
        }
    }

    bool HasValue(T val){
        head &= size;
        tail &= size;
        if (head <= tail){
            for (i = head; i < tail; i++){
                if (data[i] == val)
                    return true;
            }
        }else{
            for (i = head; i <= size; i++){
                if (data[i] == val)
                    return true;
            }
            for (i = 0; i < tail; i++){
                if (data[i] == val)
                    return true;
            }
        }
        return false;
    }
};

template <typename T>
void Free(Queue<T*> queue){
    queue.Foreach([](T* item){
        delete item;
    });
}

#endif