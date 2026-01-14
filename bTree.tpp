//
// Created by DELL on 14/01/2026.
//

#ifndef BTREE_BTREE_TPP
#define BTREE_BTREE_TPP
#include "bTree.hpp"


template<typename T>
//non full insert
void BTree<T>::insert(T key) {
if (not root){root = new Node(key);return;}
    Node* node = root;
int i=node->nodesInserted - 1;
while (i >= 0 && key < node->keys[i]) {
        node->keys[i + 1] = node->keys[i];
        --i;
    }

    // insert key
    node->keys[i + 1] = key;
    ++node->nodesInserted;

}

#endif //BTREE_BTREE_TPP
