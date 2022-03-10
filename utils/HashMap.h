#ifndef __UTILS_HASHMAP__
#define __UTILS_HASHMAP__

#include <define.h>

#include <utils/String.h>

// 此HashMap默认ANSI编码以及指针为值类型
// 默认指针不为NULL，若想设置为NULL请删除
template <typename T, const uint bitSize = 6>
class HashMapA {
private:
    struct HashNode {
        String key;
        T val;
        HashNode* next = NULL;
    };

    HashNode* nodes[(1 << bitSize)] = {NULL};

    uhash GetHash(const char* s, size_t len){
        uhash hash = len;
        for (size_t i = 0; i < len; i++)
            hash = (hash << 4) ^ (hash >> 28) ^ s[i];
        return hash & ((1 << bitSize) - 1);
    }

    void DeleteNode(HashNode* node){
        if (!node)
            return;
        DeleteNode(node->next);
        delete node;
    }

    void AddNode(HashNode*& target, const char* s, T val){
        HashNode* node = new HashNode;
        node->key = s;
        node->val = val;
        node->next = target;
        target = node;
    }

    T QueryValue(HashNode* node, const char* key){
        if (!node)
            return 0;// 可以作为NULL
        if (node->key == key)
            return node->val;
        return QueryValue(node->next, key);
    }

    HashNode* QueryNode(HashNode* node, const char* key){
        if (!node)
            return NULL;
        if (node->key == key)
            return node;
        return QueryValue(node->next, key);
    }

    void DeleteNode(HashNode*& node, const char* key){
        if (!node)
            return;
        if (node->key == key){
            delete node;
            node = node->next;
            return;
        }
        DeleteNode(node->next, key);
    }

public:
    HashMapA(){}

    ~HashMapA(){
        for (size_t i = 0; i < (1 << bitSize); i++)
            DeleteNode(nodes[i]);
    }

    T Get(const char* key){
        return QueryValue(nodes[GetHash(key, strlen(key))], key);
    }

    T Get(String& key){
        return QueryValue(nodes[GetHash(key.GetString(), key.GetLength())], key.GetString());
    }

    T Set(const char* key, T val){
        HashNode*& root = nodes[GetHash(key, strlen(key))];
        HashNode* node = QueryNode(root);
        if (node){
            T preval = node->val;
            node->val = val;
            return val;
        }
        AddNode(root, key, val);
        return 0;
    }

    T Set(String& key, T val){
        HashNode*& root = nodes[GetHash(key.GetString(), key.GetLength())];
        HashNode* node = QueryNode(root);
        if (node){
            T preval = node->val;
            node->val = val;
            return val;
        }
        AddNode(root, key.GetString(), val);
        return 0;
    }

    void Delete(const char* key){
        DeleteNode(nodes[GetHash(key, strlen(key))], key);
    }

    void Delete(String& key){
        DeleteNode(nodes[GetHash(key.GetString(), key.GetLength())], key.GetString());
    }

    HashMapA<T, bitSize>& Foreach(void(*func)(T)){
        for (size_t i = 0; i < (1 << bitSize); i++){
            HashNode* node = nodes[i];
            while (node){
                func(node->val);
                node = node->next;
            }
        }
        return *this;
    }

    template <typename Tp>
    HashMapA<T, bitSize>& Foreach(void(*func)(T, Tp), Tp user){
        for (size_t i = 0; i < (1 << bitSize); i++){
            HashNode* node = nodes[i];
            while (node){
                func(node->val, user);
                node = node->next;
            }
        }
        return *this;
    }
};

template <typename T, const uint bitSize>
void Free(HashMapA<T*, bitSize>& map){
    map.Foreach([](T* item){
        delete item;
    });
}

// 此HashMap默认UTF-16LE编码以及指针为值类型
// 默认指针不为NULL，若想设置为NULL请删除
template <typename T, const uint bitSize = 6>
class HashMapW {
private:
    struct HashNode {
        WString key;
        T val;
        HashNode* next = NULL;
    };

    HashNode* nodes[(1 << bitSize)] = {NULL};

    uhash GetHash(const wchar_t* s, size_t len){
        uhash hash = len;
        for (size_t i = 0; i < len; i++)
            hash = (hash << 4) ^ (hash >> 28) ^ s[i];
        return hash & ((1 << bitSize) - 1);
    }

    void DeleteNode(HashNode* node){
        if (!node)
            return;
        DeleteNode(node->next);
        delete node;
    }

    void AddNode(HashNode*& target, const wchar_t* s, T val){
        HashNode* node = new HashNode;
        node->key = s;
        node->val = val;
        node->next = target;
        target = node;
    }

    T QueryValue(HashNode* node, const wchar_t* key){
        if (!node)
            return 0;// 可以作为NULL
        if (node->key == key)
            return node->val;
        return QueryValue(node->next, key);
    }

    HashNode* QueryNode(HashNode* node, const wchar_t* key){
        if (!node)
            return NULL;
        if (node->key == key)
            return node;
        return QueryValue(node->next, key);
    }

    void DeleteNode(HashNode*& node, const wchar_t* key){
        if (!node)
            return;
        if (node->key == key){
            delete node;
            node = node->next;
            return;
        }
        DeleteNode(node->next, key);
    }

public:
    HashMapW(){}

    ~HashMapW(){
        for (size_t i = 0; i < (1 << bitSize); i++)
            DeleteNode(nodes[i]);
    }

    T Get(const wchar_t* key){
        return QueryValue(nodes[GetHash(key, wcslen(key))], key);
    }

    T Get(WString& key){
        return QueryValue(nodes[GetHash(key.GetString(), key.GetLength())], key.GetString());
    }

    T Set(const wchar_t* key, T val){
        HashNode*& root = nodes[GetHash(key, wcslen(key))];
        HashNode* node = QueryNode(root);
        if (node){
            T preval = node->val;
            node->val = val;
            return preval;
        }
        AddNode(root, key, val);
        return 0;
    }

    T Set(WString& key, T val){
        HashNode*& root = nodes[GetHash(key.GetString(), key.GetLength())];
        HashNode* node = QueryNode(root);
        if (node){
            T preval = node->val;
            node->val = val;
            return preval;
        }
        AddNode(root, key.GetString(), val);
    }

    void Delete(const wchar_t* key){
        DeleteNode(nodes[GetHash(key, wcslen(key))], key);
    }

    void Delete(WString& key){
        DeleteNode(nodes[GetHash(key.GetString(), key.GetLength())], key.GetString());
    }

    HashMapW<T, bitSize>& Foreach(void(*func)(T)){
        for (size_t i = 0; i < (1 << bitSize); i++){
            HashNode* node = nodes[i];
            while (node){
                func(node->val);
                node = node->next;
            }
        }
        return *this;
    }

    template <typename Tp>
    HashMapW<T, bitSize>& Foreach(void(*func)(T, Tp), Tp user){
        for (size_t i = 0; i < (1 << bitSize); i++){
            HashNode* node = nodes[i];
            while (node){
                func(node->val, user);
                node = node->next;
            }
        }
        return *this;
    }
};

template <typename T, const uint bitSize>
void Free(HashMapW<T*, bitSize>& map){
    map.Foreach([](T* item){
        delete item;
    });
}

#endif