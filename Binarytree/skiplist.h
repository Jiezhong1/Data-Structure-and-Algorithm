#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <map>

#define LEVEL 128

/*
 *                     <Skip List>
 *
 *
 * Level3  |TOP|----------------------------------|END|
 *           |                                      |
 * Level2  |TOP|----------|key2|------------------|END|
 *           |              |                       |
 * Level1  |TOP|--|key1|--|key2|----------|key4|--|END|
 *           |      |       |               |       |
 * Level0  |TOP|--|key1|--|key2|--|key3|--|key4|--|END|
 *
 *         Node    Node    Node    Node    Node   Node
 *
 */

template<typename KeyType, typename ValueType>
struct skiplist {
    KeyType key;
    ValueType value;
    int level;    // 最大Level
    std::pair<struct skiplist<KeyType, ValueType> *, struct skiplist<KeyType, ValueType> *> *neighbor;
};

template<typename KeyType, typename ValueType>
class SkipList {
    public:
        SkipList();
        ~SkipList();

        skiplist<KeyType, ValueType> *get(KeyType key);
        skiplist<KeyType, ValueType> *put(KeyType key, ValueType value);
        int remove(KeyType key);
    private:
        inline unsigned int upsides();    // ノード毎に最大Levelを決定する(Levelは1/2の確率で1上がる)
        skiplist<KeyType, ValueType> *top;
        skiplist<KeyType, ValueType> *end;
};

template<typename KeyType, typename ValueType>
SkipList<KeyType, ValueType>::SkipList()
{
    srand(time(NULL));

    top = new skiplist<KeyType, ValueType>;
    end = new skiplist<KeyType, ValueType>;
    
    top->key   = 0;
    top->value = 0;
    top->level = LEVEL;
    end->key   = 0xffffffff;
    end->value = 0;
    end->level = LEVEL;

    top->neighbor = new std::pair<struct skiplist<KeyType, ValueType> *, struct skiplist<KeyType, ValueType> *>[LEVEL];
    end->neighbor = new std::pair<struct skiplist<KeyType, ValueType> *, struct skiplist<KeyType, ValueType> *>[LEVEL];

    for(int level=0; level < LEVEL; level++) {
        top->neighbor[level].first  = NULL;
        top->neighbor[level].second = end;

        end->neighbor[level].first  = top;
        end->neighbor[level].second = NULL;
    }
}

template<typename KeyType, typename ValueType>
SkipList<KeyType, ValueType>::~SkipList()
{
    skiplist<KeyType, ValueType> *p;
    skiplist<KeyType, ValueType> *next;

    if(top) {
        for(p = top; p; p = next) {
            next = p->neighbor[0].second;

            delete[] p->neighbor;
            delete p;
        }
    }
}

// Hit        =>  Node
// Not found  =>  Previous
template<typename KeyType, typename ValueType>
skiplist<KeyType, ValueType> *SkipList<KeyType, ValueType>::get(KeyType key)
{
    skiplist<KeyType, ValueType> *p = top->neighbor[0].second;

    if(p == end) {
        if(end->key == key) {
            return end;
        }
        return top;
    }

    while(p != end) {
        if(p->key == key) {
            return p;
        } else if(p->key < key && key <= p->neighbor[0].second->key) {
            // keyがpより大きく、右側のノードよりも小さい
            if(p->neighbor[0].second->key == key) {
                return p->neighbor[0].second;
            }
            return p;
        } else if(p->neighbor[0].first->key <= key && key < p->key) {
            // keyがpより小さく、左側のノードよりも小さい
            return p->neighbor[0].first;
        } else if(p->key < key) {
            // keyがpより大きく、さらに右側のノードよりも大きい場合
            int level;
            for(level=1; level < p->level; level++) {
                if(key <= p->neighbor[level].second->key) break;
            }

            if(level < p->level) {
                if(p->neighbor[level].second->key == key) {
                    return p->neighbor[level].second;
                }
            }
            p = p->neighbor[level - 1].second;
        } else if(key < p->key) {
            // keyがpより小さく、さらに左側のノードよりも小さい場合
            int level;
            for(level=1; level < p->level; level++) {
                if(p->neighbor[level - 1].first->key <= key) break;
            }

            p = p->neighbor[level - 1].first;
        }
    }
}

template<typename KeyType, typename ValueType>
skiplist<KeyType, ValueType> *SkipList<KeyType, ValueType>::put(KeyType key, ValueType value)
{
    skiplist<KeyType, ValueType> *p = get(key);

    if(p->key == key) {
        p->value = value;
        return p;
    } else {
        // もしkeyが存在しなければ新しく挿入する

        skiplist<KeyType, ValueType> *new_node = new skiplist<KeyType, ValueType>;
        new_node->key      = key;
        new_node->value    = value;
        new_node->level    = upsides();    // 最大Levelを決定
        new_node->neighbor = new std::pair<struct skiplist<KeyType, ValueType> *, struct skiplist<KeyType, ValueType> *>[ new_node->level ];

        skiplist<KeyType, ValueType> *left  = p;
        skiplist<KeyType, ValueType> *right = left->neighbor[0].second;
        new_node->neighbor[0].first                     = left;
        new_node->neighbor[0].second                    = right;
        left->neighbor[0].second                        = new_node;
        right->neighbor[0].first                        = new_node;

        struct {
            skiplist<KeyType, ValueType> *left;
            skiplist<KeyType, ValueType> *right;
        } edge = {left, right};

        for(int level=1; level < new_node->level; level++) {
            // 左側のノードとリンクする
            while(edge.left->level <= level) {
                edge.left = edge.left->neighbor[ edge.left->level - 1 ].first;
            }
            new_node->neighbor[level].first   = edge.left;
            new_node->neighbor[level].second  = edge.left->neighbor[level].second;
            edge.left->neighbor[level].second = new_node;

            if(edge.right != end) {
                // 右側のノードとリンクする
                while(edge.right->level <= level) {
                    edge.right = edge.right->neighbor[ edge.right->level - 1 ].second;
                }
                new_node->neighbor[level].second = edge.right;
                new_node->neighbor[level].first  = edge.right->neighbor[level].first;
                edge.left->neighbor[level].first = new_node;
            }
        }

        return new_node;
    }
}

template<typename KeyType, typename ValueType>
int SkipList<KeyType, ValueType>::remove(KeyType key)
{
    skiplist<KeyType, ValueType> *p = get(key);

    if(p->key != key) {
        return 0;
    }

    for(int level = 0; level < p->level; level++) {
        p->neighbor[level].first->neighbor[level].second = p->neighbor[level].second;
        p->neighbor[level].second->neighbor[level].first = p->neighbor[level].first;
    }

    delete[] p->neighbor;
    delete p;

    return 1;
}

// ノード毎に最大Levelを決定する(Levelは1/2の確率で1上がる)
template<typename KeyType, typename ValueType>
inline unsigned int SkipList<KeyType, ValueType>::upsides()
{
    for(int level = 1; level < LEVEL; level++)
        if(rand() % 2 == 0) return level;

    return LEVEL;
}