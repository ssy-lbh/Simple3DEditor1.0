#ifndef __UTILS_RBTREE__
#define __UTILS_RBTREE__

#include <define.h>

// 红黑树节点
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

    // 还是查找最简单，直接比较然后找左右子节点
    RBTreeNode<K, V>* Query(K key){
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

    // 左旋:                右旋
    //    1           1           1       1
    //    |           |           |       |
    //    2     =>    3           2 =>    3
    //     \         /           /         \
    //      3       2           3           2
    //     /         \           \         /
    //    4           4           4       4
    // 2号为此节点
    void Rotate(bool left){
        RBTreeNode<K, V>* t = son[left];
        if (father){
            // 将子节点换到自己的位置
            father->son[IsRight()] = t;
        }
        t->father = father;
        father = t;
        son[left] = t->son[!left];
        // 左旋后为左节点，右旋后为右节点
        t->son[!left] = this;
    }

    // 红黑树规则：红色不相邻，黑色可相邻，根节点必须为黑色
    // 路径上黑节点数量决定了最大深度和最小深度差别不会大于两倍
    // 每一次都是变红的节点才需要刷新
    void Update(RBTreeNode<K, V>*& root){
        while (root->father)
            root = root->father;
        // 根节点固定为黑色，如果被变红则在这里变回
        root->black = true;
        // 刚刚插入的节点为红色，因此在父节点存在且为红时才需要更新
        // 不存在时也就是根节点了
        if (father && !father->black){
            // 相邻节点存在且为红，则父节点变黑，父节点相邻节点变黑，祖父节点变红并刷新
            RBTreeNode<K, V>* rel = father->Relative();
            if (rel && !rel->black){
                father->black = true;
                rel->black = true;
                father->father->black = false;
                father->father->Update(root);
                return;
            } else {
                // 相邻节点若为黑则与没有没区别，因为无论放哪都不违规
                bool right = IsRight();
                // 从祖父到此节点路径拐弯时，旋转成直线，再由底部父节点刷新
                if (father->IsRight() ^ right){
                    father->Rotate(right);
                    // 原来的父节点刷新
                    son[!right]->Update(root);
                    return;
                }
                // 路径不拐弯，将父节点设为黑，祖父节点设为红再旋转
                // 变成父节点为黑，原先的祖父节点与此节点为红并为其子节点
                father->black = true;
                father->father->black = false;
                father->father->Rotate(right);
                return;
            }
        }
    }

    RBTreeNode<K, V>* Insert(RBTreeNode<K, V>*& root, K key, V val){
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
};

// 红黑树
template <typename K, typename V>
class RBTree {
public:
    RBTreeNode<K, V>* root;

    RBTree(){}
    ~RBTree(){}

    RBTreeNode<K, V>* Query(K key){
        return root->Query(key);
    }

    RBTreeNode<K, V>* Insert(K key, V val){
        if(!root)
            return (root = RBTreeNode<K, V>(key, val, NULL, true));
        RBTreeNode<K, V>* ret = root->Insert(root, key, val);
        while (root->father)
            root = root->father;
        root->black = true;
        return ret;
    }

    void Replace(RBTreeNode<K, V>* target, RBTreeNode<K, V>*& src){
        target->key = src->key;
        target->val = src->val;
        delete src;
        src = NULL;
    }

    void Delete(K key){
        if (!root)
            return;
        RBTreeNode<K, V>* target = root->Query(key);
        if (!target)
            return;
        if (target->son[0]){
            // 存在两个子节点，我就随便找到右侧值最邻近的节点代替自身
            if (target->son[1]){
                RBTreeNode<K, V>** src = &target->son[1];
                while ((*src)->son[0])
                    src = &(*src)->son[0];
                Replace(target, *src);
                return;
            }
            // 只有一个左节点，以子节点替换自身
            Replace(target, target->son[0]);
        } else if (target->son[1]){
            // 只有一个右节点，以子节点替换自身
            Replace(target, target->son[1]);
        } else {
            // 都没有，直接删除
            if (target == root)
                root = NULL;
            delete target;
        }
    }
};

#endif