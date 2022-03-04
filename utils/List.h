#ifndef __UTILS_LIST__
#define __UTILS_LIST__

#include <define.h>

#include <utils/os/Log.h>

template <typename T>
class List : public Object {
private:
    T* data;
    size_t size;
    size_t ptr;

    void Check(size_t reserve){
        if (ptr + reserve < size){
            return;
        }
        while (ptr + reserve >= size){
            size <<= 1;
        }
        //DebugLog("List %p Size Increased %llu", this, size);
        T* newData = new T[size];
        for (size_t i = 0; i < ptr; i++){
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    List(){
        //DebugLog("List()");
        size = 8;
        data = new T[size];
        ptr = 0;
    }

    List(size_t size){
        this->size = size;
        data = new T[size];
        ptr = 0;
    }

    List(List&& list){
        //DebugLog("List(List&& list)");
        size = list.size;
        data = new T[size];
        ptr = list.ptr;
        for (size_t i = 0; i < ptr; i++){
            data[i] = list.data[i];
        }
    }

    List(const List& list){
        //DebugLog("List(const List& list)");
        size = list.size;
        data = new T[size];
        ptr = list.ptr;
        for (size_t i = 0; i < ptr; i++){
            data[i] = list.data[i];
        }
    }

    List& operator=(List&& list){
        //DebugLog("List& operator=(List&& list)");
        delete[] data;
        size = list.size;
        data = new T[size];
        ptr = list.ptr;
        for (size_t i = 0; i < ptr; i++){
            data[i] = list.data[i];
        }
        return *this;
    }

    List& operator=(const List& list){
        //DebugLog("List& operator=(const List& list)");
        delete[] data;
        size = list.size;
        data = new T[size];
        ptr = list.ptr;
        for (size_t i = 0; i < ptr; i++){
            data[i] = list.data[i];
        }
        return *this;
    }

    ~List(){
        //DebugLog("~List()");
        if (data == NULL){
            DebugError("Critical: List<T>::~List Data Pointer Is NULL");
        }
        delete[] data;
    }

    List<T>& Add(T val){
        //DebugLog("Add");
        Check(5);
        data[ptr++] = val;
        return *this;
    }

    List<T>& Insert(size_t index, T val){
        if (index > ptr){
            DebugError("Critical: List<T>::Insert(%llu) When Index Overflow", index);
            return *this;
        }
        Check(5);
        for (size_t i = ptr; i > index; i--){
            data[i] = data[i - 1];
        }
        ptr++;
        data[index] = val;
        return *this;
    }

    T RemoveBack(){
        //DebugLog("Clear");
        if (ptr == 0){
            DebugError("Critical: List<T>::RemoveBack When Size Is 0");
            return *data;
        }
        return data[--ptr];
    }

    size_t Size(){
        return ptr;
    }

    bool Empty(){
        return ptr == 0;
    }

    T& GetFront(){
        //DebugLog("GetFront");
        if (ptr == 0){
            DebugError("Critical: List<T>::GetFront When Size Is 0");
        }
        return *data;
    }

    T& GetBack(){
        //DebugLog("GetBack");
        if (ptr == 0){
            DebugError("Critical: List<T>::GetBack When Size Is 0");
            return *data;
        }
        return data[ptr - 1];
    }

    T* GetData(){
        return data;
    }

    T& operator[](size_t index){
        if (index >= ptr){
            DebugError("Critical: List<T>::operator[](%llu) When Index Overflow", index);
        }
        return data[index];
    }

    T& GetItem(size_t index){
        //DebugLog("GetItem %d", index);
        if (index >= ptr){
            DebugError("Critical: List<T>::GetItem(%llu) When Index Overflow", index);
        }
        return data[index];
    }

    void Clear(){
        ptr = 0;
    }

    void Clear(size_t memory){
        delete[] data;
        size = memory;
        data = new T[size];
        ptr = 0;
        if (data == NULL){
            DebugError("Critical: List<T>::Clear(%d) Allocate Memory Failed", memory);
        }
    }

    List<T>& RemoveAt(size_t index){
        if (index >= ptr){
            DebugError("Critical: List<T>::RemoveAt(%llu) When Index Overflow", index);
            return *this;
        }
        ptr--;
        for (size_t i = index; i < ptr; i++){
            data[i] = data[i + 1];
        }
        return *this;
    }

    bool Remove(T val){
        //DebugLog("Remove");
        for (size_t i = 0; i < ptr; ++i){
            if (data[i] == val){
                ptr--;
                for (size_t j = i; j < ptr; ++j)
                    data[j] = data[j + 1];
                return true;
            }
        }
        return false;
    }

    List<T>& Foreach(void(*func)(T)){
        //DebugLog("Foreach");
        for (size_t i = 0; i < ptr; i++){
            func(data[i]);
        }
        return *this;
    }

    template <typename Tp>
    List<T>& Foreach(void(*func)(T, Tp), Tp user){
        //DebugLog("Foreach");
        for (size_t i = 0; i < ptr; i++){
            func(data[i], user);
        }
        return *this;
    }

    bool HasValue(T val){
        for (size_t i = 0; i < ptr; ++i){
            if (data[i] == val){
                return true;
            }
        }
        return false;
    }
};

template <typename T>
void Free(List<T*> list){
    list.Foreach([](T* item){
        delete item;
    });
}

#endif