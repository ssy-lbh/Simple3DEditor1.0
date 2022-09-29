#ifndef __UTIL_HASHMAP__
#define __UTIL_HASHMAP__

#include <define.h>

#include <cstring>
#include <functional>

#include <util/String.h>

namespace simple3deditor {

// ��ϣ���������

// ��HashMapĬ��ANSI�����Լ�ָ��Ϊֵ����
// Ĭ��ָ�벻ΪNULL����������ΪNULL��ɾ��
template <typename T, const uint BIT_SIZE = 6>
class HashMapA {
private:
    static constexpr uint NODE_SIZE = (1 << BIT_SIZE);
    static constexpr uint NODE_MASK = NODE_SIZE - 1;

    struct HashNode {
        String key;
        T val;
        HashNode* next = NULL;
    };

    HashNode* nodes[NODE_SIZE] = {NULL};

    uhash GetHash(const char* s, size_t len){
        uhash hash = len;
        for (size_t i = 0; i < len; i++)
            hash = (hash << 4) ^ (hash >> 28) ^ s[i];
        return hash & NODE_MASK;
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
        node = QueryNode(node, key);
        return node == NULL ? 0 : node->val;
    }

    HashNode* QueryNode(HashNode* node, const char* key){
        if (!node)
            return NULL;
        if (node->key == key)
            return node;
        return QueryNode(node->next, key);
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
        for (size_t i = 0; i < NODE_SIZE; i++)
            DeleteNode(nodes[i]);
    }

    T Get(const char* key){
        return QueryValue(nodes[GetHash(key, strlen(key))], key);
    }

    T Get(const String& key){
        return QueryValue(nodes[GetHash(key.GetString(), key.GetLength())], key.GetString());
    }

    T Set(const char* key, T val){
        HashNode*& root = nodes[GetHash(key, strlen(key))];
        HashNode* node = QueryNode(root, key);
        if (node){
            T preval = node->val;
            node->val = val;
            return val;
        }
        AddNode(root, key, val);
        return 0;
    }

    T Set(const String& key, T val){
        HashNode*& root = nodes[GetHash(key.GetString(), key.GetLength())];
        HashNode* node = QueryNode(root, key.GetString());
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

    void Delete(const String& key){
        DeleteNode(nodes[GetHash(key.GetString(), key.GetLength())], key.GetString());
    }

    HashMapA<T, BIT_SIZE>& Foreach(std::function<void(const String&, T)> func){
        for (size_t i = 0; i < NODE_SIZE; i++){
            HashNode* node = nodes[i];
            while (node){
                func(node->key, node->val);
                node = node->next;
            }
        }
        return *this;
    }
};

template <typename T, const uint BIT_SIZE>
void Free(HashMapA<T*, BIT_SIZE>& map){
    map.Foreach([](const String& key, T* item){
        delete item;
    });
}

// ��HashMapĬ��UTF-16LE�����Լ�ָ��Ϊֵ����
// Ĭ��ָ�벻ΪNULL����������ΪNULL��ɾ��
template <typename T, const uint BIT_SIZE = 6>
class HashMapW {
private:
    static constexpr uint NODE_SIZE = (1 << BIT_SIZE);
    static constexpr uint NODE_MASK = NODE_SIZE - 1;

    struct HashNode {
        WString key;
        T val;
        HashNode* next = NULL;
    };

    HashNode* nodes[NODE_SIZE] = {NULL};

    uhash GetHash(const wchar_t* s, size_t len){
        uhash hash = len;
        for (size_t i = 0; i < len; i++)
            hash = (hash << 4) ^ (hash >> 28) ^ s[i];
        return hash & NODE_MASK;
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
        node = QueryNode(node, key);
        return node == NULL ? 0 : node->val;
    }

    HashNode* QueryNode(HashNode* node, const wchar_t* key){
        if (!node)
            return NULL;
        if (node->key == key)
            return node;
        return QueryNode(node->next, key);
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
        for (size_t i = 0; i < NODE_SIZE; i++)
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
        HashNode* node = QueryNode(root, key);
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
        HashNode* node = QueryNode(root, key.GetString());
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

    HashMapW<T, BIT_SIZE>& Foreach(std::function<void(const WString&, T)> func){
        for (size_t i = 0; i < NODE_SIZE; i++){
            HashNode* node = nodes[i];
            while (node){
                func(node->key, node->val);
                node = node->next;
            }
        }
        return *this;
    }
};

template <typename T, const uint BIT_SIZE>
void Free(HashMapW<T*, BIT_SIZE>& map){
    map.Foreach([](const WString& key, T* item){
        delete item;
    });
}

}

#endif