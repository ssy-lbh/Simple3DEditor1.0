#ifndef __LIST__
#define __LIST__

#include <vector>

template <typename T>
class List {
private:
    std::vector<T> data;

public:
    List(){}
    ~List(){}

    List<T>& Add(T val){
        data.push_back(val);
        return *this;
    }

    size_t Size(){
        return data.size();
    }

    T GetFront(){
        return data.front();
    }

    T GetItem(int index){
        if (index >= data.size() || index < 0){
            return NULL;
        }
        return data[index];
    }

    void Clear(){
        data.clear();
    }

    List<T>& Foreach(void(*func)(T)){
        for (decltype(data.begin()) i = data.begin(); i != data.end(); ++i){
            func(*i);
        }
        return *this;
    }

    template <typename Tp>
    List<T>& Foreach(void(*func)(T, Tp), Tp user){
        for (decltype(data.begin()) i = data.begin(); i != data.end(); ++i){
            func(*i, user);
        }
        return *this;
    }
};

template <typename T>
void Free(List<T*> list){
    list.Foreach([](T* item){
        delete item;
    });
}

#endif