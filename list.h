#ifndef __LIST__
#define __LIST__

#include "define.h"

#include "log.h"

template <typename T>
class List {
private:
    T* data;
    unsigned long long size;
    unsigned long long ptr;

    void Check(size_t reserve){
        if (ptr + reserve < size){
            return;
        }
        //DebugLog("List Size Increased");
        while (ptr + reserve >= size){
            size <<= 1;
        }
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
        size = 32;
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
        size = list.size;
        data = new T[size];
        ptr = list.ptr;
        for (size_t i = 0; i < ptr; i++){
            data[i] = list.data[i];
        }
    }

    List& operator=(const List& list){
        //DebugLog("List& operator=(const List& list)");
        size = list.size;
        data = new T[size];
        ptr = list.ptr;
        for (size_t i = 0; i < ptr; i++){
            data[i] = list.data[i];
        }
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
        Check(1);
        data[ptr++] = val;
        return *this;
    }

    T RemoveBack(){
        //DebugLog("Clear");
        if (ptr == 0){
            DebugError("Critical: List<T>::RemoveBack When Size Is 0");
        }
        return data[--ptr];
    }

    size_t Size(){
        return ptr;
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
        }
        return data[ptr - 1];
    }

    T& GetItem(size_t index){
        //DebugLog("GetItem %d", index);
        if (index >= ptr){
            DebugError("Critical: List<T>::GetItem When Index Overflow");
        }
        return data[index];
    }

    void Clear(){
        //DebugLog("Clear");
        delete data;
        size = 32;
        data = new T[size];
        ptr = 0;
        if (data == NULL){
            DebugError("Critical: List<T>::Clear Allocate Memory Failed");
        }
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