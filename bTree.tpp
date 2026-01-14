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
    if (node->nodesInserted==3) {
        split(node);
       
    }
int i=node->nodesInserted - 1;
while (i >= 0 && key < node->keys[i]) {
        node->keys[i + 1] = node->keys[i];
        --i;
    }
    // insert key
    node->keys[i + 1] = key;
    ++node->nodesInserted;
}

template<typename T>
void BTree<T>::split(Node *node) {
    if (node==root) {
        Node* Nroot = new Node(node->keys[1]);
        Node* left = new Node(node->keys[0]);
        Node* right = new Node(node->keys[2]);
        Nroot->children[0] = left;
        Nroot->children[1]=  right;
        root = Nroot;
        left ->children[0] = node->children[0];
        left ->children[1] = node->children[1];
        right ->children[0] = node->children[2];
        right ->children[1] = node->children[3];
        delete node;
        return;
    }
    Node* left = new Node(node->keys[0]);
    Node* right = new Node(node->keys[2]);
}
#endif //BTREE_BTREE_TPP
