#ifndef __UTILS_TRIE__
#define __UTILS_TRIE__

#include <define.h>

#include <utils/String.h>

// 此Trie默认ANSI编码
// T最好是指针，可以判断结尾，不浪费内存，携带数据多，速度快
// T如果是基本数据类型，查不到值时默认为0，不便于判断结尾
// T不能是结构体或者类，初始化和判断结尾都有问题
template <typename T>
class TrieA {
private:
    TrieA* nodes[0x10] = {NULL};
    T data = 0;// 如果是指针则默认为NULL

public:
    TrieA(){}

    ~TrieA(){
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                delete nodes[i];
    }

    // this不能为NULL，因为this是只读的，不能完成创建操作
    TrieA* Create(const char* s, size_t len){
        TrieA* root = this;
        TrieA** node = &root;
        for (size_t i = 0; i < len; i++){
            node = &(*node)->nodes[s[i] & 0x0F];
            if (!*node)
                *node = new TrieA();
            node = &(*node)->nodes[(s[i] & 0xF0) >> 4];
            if (!*node)
                *node = new TrieA();
        }
        return *node;
    }

    TrieA* Create(const char* s){
        return Create(s, strlen(s));
    }

    TrieA* Create(String& s){
        return Create(s.GetString(), s.GetLength());
    }

    // this可以为NULL
    TrieA* Query(const char* s, size_t len){
        TrieA* node = this;
        for (size_t i = 0; i < len; i++){
            if (!node)
                return NULL;
            node = node->nodes[s[i] & 0x0F];
            if (!node)
                return NULL;
            node = node->nodes[(s[i] & 0xF0) >> 4];
        }
        return node;
    }

    TrieA* Query(const char* s){
        return Query(s, strlen(s));
    }

    TrieA* Query(String& s){
        return Query(s.GetString(), s.GetLength());
    }

    bool Empty(){
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                return false;
        return true;
    }

    // this为NULL时也可以调用
    T Data(){
        return (this == NULL ? 0 : data);
    }

    T Get(const char* key){
        return Query(key)->Data();
    }

    T Get(String& key){
        return Query(key)->Data();
    }

    void Set(const char* key, T val){
        Create(key)->data = val;
    }

    void Set(String& key, T val){
        Create(key)->data = val;
    }
    
    // 模板函数默认为inline，这样的话我真不知道咋办
    void Foreach(void(*func)(T)){
        if (data)
            func(data);
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                nodes[i]->Foreach(func);
    }

    template <typename Tp>
    void Foreach(void(*func)(T, Tp), Tp user){
        if (data)
            func(data, user);
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                nodes[i]->Foreach(func, user);
    }
};

template <typename T>
void Free(TrieA<T*>& trie){
    trie.Foreach([](T* item){
        delete item;
    });
}

// 此Trie默认UTF-16LE编码
// T最好是指针，可以判断结尾，不浪费内存，携带数据多，速度快
// T如果是基本数据类型，查不到值时默认为0，不便于判断结尾
// T不能是结构体或者类，初始化和判断结尾都有问题
template <typename T>
class TrieW {
private:
    TrieW* nodes[0x10] = {NULL};
    T data = 0;// 如果是指针则默认为NULL

public:
    TrieW(){}

    ~TrieW(){
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                delete nodes[i];
    }

    // this不能为NULL，因为this是只读的，不能完成创建操作
    TrieW* Create(const wchar_t* s, size_t len){
        TrieW* root = this;
        TrieW** node = &root;
        for (size_t i = 0; i < len; i++){
            node = &(*node)->nodes[s[i] & 0x0F];
            if (!*node)
                *node = new TrieW();
            node = &(*node)->nodes[(s[i] & 0xF0) >> 4];
            if (!*node)
                *node = new TrieW();
        }
        return *node;
    }

    TrieW* Create(const wchar_t* s){
        return Create(s, wcslen(s));
    }

    TrieW* Create(WString& s){
        return Create(s.GetString(), s.GetLength());
    }

    // this可以为NULL
    TrieW* Query(const wchar_t* s, size_t len){
        TrieW* node = this;
        for (size_t i = 0; i < len; i++){
            if (!node)
                return NULL;
            node = node->nodes[s[i] & 0x0F];
            if (!node)
                return NULL;
            node = node->nodes[(s[i] & 0xF0) >> 4];
        }
        return node;
    }

    TrieW* Query(const wchar_t* s){
        return Query(s, wcslen(s));
    }

    TrieW* Query(WString& s){
        return Query(s.GetString(), s.GetLength());
    }

    bool Empty(){
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                return false;
        return true;
    }

    // this为NULL时也可以调用
    T Data(){
        return (this == NULL ? 0 : data);
    }

    T Get(const wchar_t* key){
        return Query(key)->Data();
    }

    T Get(WString& key){
        return Query(key)->Data();
    }

    void Set(const wchar_t* key, T val){
        Create(key)->data = val;
    }

    void Set(WString& key, T val){
        Create(key)->data = val;
    }
    
    // 模板函数默认为inline，这样的话我真不知道咋办
    void Foreach(void(*func)(T)){
        if (data)
            func(data);
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                nodes[i]->Foreach(func);
    }

    template <typename Tp>
    void Foreach(void(*func)(T, Tp), Tp user){
        if (data)
            func(data, user);
        for (uchar i = 0; i < 0x10; i++)
            if (nodes[i])
                nodes[i]->Foreach(func, user);
    }
};

template <typename T>
void Free(TrieW<T*>& trie){
    trie.Foreach([](T* item){
        delete item;
    });
}

#endif