#ifndef __UTIL_RBTREE__
#define __UTIL_RBTREE__

#include <define.h>

#include <functional>

namespace simple3deditor {

// ������ڵ�
template <typename K, typename V>
class RBTreeNode {
public:
    RBTreeNode<K, V>* son[2] = {NULL, NULL};
    RBTreeNode<K, V>* father;
    K key;
    V val;
    bool black;

    RBTreeNode<K, V>(){}

    RBTreeNode<K, V>(K key, V val, RBTreeNode<K, V>* father = NULL, bool black = false){
        this->key = key;
        this->val = val;
        this->father = father;
        this->black = black;
    }

    ~RBTreeNode<K, V>(){
        if (son[0])
            delete son[0];
        if (son[1])
            delete son[1];
    }

    // ���ǲ�����򵥣�ֱ�ӱȽ�Ȼ���������ӽڵ�
    RBTreeNode<K, V>* Query(const K& key){
        if (this->key == key)
            return this;
        RBTreeNode<K, V>* next = son[this->key < key];
        return next ? next->Query(key) : NULL;
    }

    bool IsLeft(){
        return father->son[0] == this;
    }

    bool IsRight(){
        return father->son[1] == this;
    }

    RBTreeNode<K, V>* Relative(){
        return father->son[IsLeft()];
    }

    // ����:                ����
    //    1           1           1       1
    //    |           |           |       |
    //    2     =>    3           2 =>    3
    //     \         /           /         \
    //      3       2           3           2
    //     /         \           \         /
    //    4           4           4       4
    // 2��Ϊ�˽ڵ�
    void Rotate(bool left){
        RBTreeNode<K, V>* t = son[left];
        if (father){
            // ���ӽڵ㻻���Լ���λ��
            father->son[IsRight()] = t;
        }
        t->father = father;
        father = t;
        son[left] = t->son[!left];
        // ������Ϊ��ڵ㣬������Ϊ�ҽڵ�
        t->son[!left] = this;
    }

    // ��������򣺺�ɫ�����ڣ���ɫ�����ڣ����ڵ����Ϊ��ɫ
    // ·���Ϻڽڵ����������������Ⱥ���С��Ȳ�𲻻��������
    // ÿһ�ζ��Ǳ��Ľڵ����Ҫˢ��
    void Update(RBTreeNode<K, V>*& root){
        while (root->father)
            root = root->father;
        // ���ڵ�̶�Ϊ��ɫ��������������������
        root->black = true;
        // �ող���Ľڵ�Ϊ��ɫ������ڸ��ڵ������Ϊ��ʱ����Ҫ����
        // ������ʱҲ���Ǹ��ڵ���
        if (father && !father->black){
            // ���ڽڵ������Ϊ�죬�򸸽ڵ��ڣ����ڵ����ڽڵ��ڣ��游�ڵ��첢ˢ��
            RBTreeNode<K, V>* rel = father->Relative();
            if (rel && !rel->black){
                father->black = true;
                rel->black = true;
                father->father->black = false;
                father->father->Update(root);
                return;
            } else {
                // ���ڽڵ���Ϊ������û��û������Ϊ���۷��Ķ���Υ��
                bool right = IsRight();
                // ���游���˽ڵ�·������ʱ����ת��ֱ�ߣ����ɵײ����ڵ�ˢ��
                if (father->IsRight() ^ right){
                    father->Rotate(right);
                    // ԭ���ĸ��ڵ�ˢ��
                    son[!right]->Update(root);
                    return;
                }
                // ·�������䣬�����ڵ���Ϊ�ڣ��游�ڵ���Ϊ������ת
                // ��ɸ��ڵ�Ϊ�ڣ�ԭ�ȵ��游�ڵ���˽ڵ�Ϊ�첢Ϊ���ӽڵ�
                father->black = true;
                father->father->black = false;
                father->father->Rotate(right);
                return;
            }
        }
    }

    RBTreeNode<K, V>* Insert(RBTreeNode<K, V>*& root, const K& key, V val){
        if (this->key == key){
            this->val = val;
            return this;
        }
        RBTreeNode<K, V>*& node = son[this->key < key];
        if (!node){
            node = new RBTreeNode<K, V>(key, val, this, false);
            node->Update(root);
            return node;
        }
        return node->Insert(root, key, val);
    }

    RBTreeNode<K, V>& Foreach(std::function<void(const K&, V)>& f){
        if (son[0])
            son[0]->Foreach(f);
        f(key, val);
        if (son[1])
            son[1]->Foreach(f);
        return *this;
    }
};

// �����
template <typename K, typename V>
class RBTree {
private:
    void Replace(RBTreeNode<K, V>* target, RBTreeNode<K, V>*& src){
        target->key = src->key;
        target->val = src->val;
        delete src;
        src = NULL;
    }

public:
    RBTreeNode<K, V>* root = NULL;

    RBTree(){}

    ~RBTree(){
        if (root)
            delete root;
    }

    RBTreeNode<K, V>* Query(const K& key){
        return root->Query(key);
    }

    RBTreeNode<K, V>* Insert(const K& key, V val){
        if(!root)
            return (root = new RBTreeNode<K, V>(key, val, NULL, true));
        RBTreeNode<K, V>* ret = root->Insert(root, key, val);
        while (root->father)
            root = root->father;
        root->black = true;
        return ret;
    }

    void Delete(const K& key){
        if (!root)
            return;
        RBTreeNode<K, V>* target = root->Query(key);
        if (!target)
            return;
        if (target->son[0]){
            // ���������ӽڵ㣬�Ҿ�����ҵ��Ҳ�ֵ���ڽ��Ľڵ��������
            if (target->son[1]){
                RBTreeNode<K, V>** src = &target->son[1];
                while ((*src)->son[0])
                    src = &(*src)->son[0];
                Replace(target, *src);
                return;
            }
            // ֻ��һ����ڵ㣬���ӽڵ��滻����
            Replace(target, target->son[0]);
        } else if (target->son[1]){
            // ֻ��һ���ҽڵ㣬���ӽڵ��滻����
            Replace(target, target->son[1]);
        } else {
            // ��û�У�ֱ��ɾ��
            if (target == root)
                root = NULL;
            delete target;
        }
    }

    RBTree<K, V>& Foreach(std::function<void(const K&, V)> f){
        if (root)
            root->Foreach(f);
        return *this;
    }

    void Free(){
        if (root){
            delete root;
            root = NULL;
        }
    }
};

template <typename K, typename V>
void Free(RBTree<K, V>& tree){
    tree.Free();
}

}

#endif