//
// Created by DELL on 14/01/2026.
//

#ifndef BTREE_BTREE_TPP
#define BTREE_BTREE_TPP
#include "bTree.hpp"
#include <cassert>

template<typename T>
void BTree<T>::insert(T key) {
if (not root){root = new Node(key);return;}
    Node* current = root;

    while (current) {
        int j;
        for (j =0; j< current->nodesInserted; j++) {

                if (current->children[j] && current->children[j]->nodesInserted == MAX_KEYS && key < current->keys[j]){
                   T promotedKey = split(current->children[j]);
                       Node* leftChild = current->children[j];
                       Node* rightChild = current->children[j+1];
                       if (leftChild == nullptr || rightChild == nullptr) {
                           break;
                       }
                    if (key<promotedKey) {
                        current = leftChild;
                    }
                    else {
                        current = rightChild;
                    }
                    break;
                }
                current = current->children[j];

        }
        if (j == current->nodesInserted) {
            if (current->children[current->nodesInserted] and
     current->children[current->nodesInserted]->nodesInserted == MAX_KEYS)
            {
               T promotedKey = split(current->children[current->nodesInserted]);
                Node* leftChild = current->children[current->nodesInserted];
                Node* rightChild = current->children[current->nodesInserted + 1];
                if (!leftChild || !rightChild) break;
                if (key<promotedKey)current = leftChild;

                else current = rightChild;

                break;
            }
            current = current->children[current->nodesInserted];
        }
    }
        int i=current->nodesInserted - 1;
        while (i >= 0 && key < current->keys[i]) {
            current->keys[i + 1] = current->keys[i];
            --i;
        }
        // insert key
        current->keys[i + 1] = key;
        ++current->nodesInserted;
}

template<typename T>
T BTree<T>::split(Node*node) {
    if (node==root) {
        Node* Nroot = new Node(node->keys[1]);
        Node* left = new Node(node->keys[0]);
        Node* right = new Node(node->keys[2]);
        Nroot->children[0] = left;
        Nroot->children[1]=  right;
        root = Nroot;
        left->parent = Nroot;
        right->parent = Nroot;
        left ->children[0] = node->children[0];
        left ->children[1] = node->children[1];
        right ->children[0] = node->children[2];
        right ->children[1] = node->children[3];
        if (left->children[0])left->children[0]->parent = left;
        if (left->children[1])  left->children[1]->parent = left;
        if (right->children[0]) right ->children[0]->parent =right;
        if (right->children[1]) right ->children[1]->parent = right;
        delete node;
        return T();
    }
    Node* parent = node->parent;
    int i=parent->nodesInserted - 1;
    T val = node->keys[1];
    while (i >= 0 && val < parent->keys[i]) {
        parent->keys[i + 1] = parent->keys[i];
        --i;
    }
    parent->keys[i + 1] = val;
    Node* left = new Node(node->keys[0]);
    Node* right = new Node(node->keys[2]);
    int idx = 0;
    while (idx < parent->nodesInserted + 1 && parent->children[idx] != node) {
        assert(idx <= parent->nodesInserted);
        idx++;
    }
    const int childCount = parent->nodesInserted +1;
    for (int k = childCount-1;k > idx; --k) {
        parent->children[k+1] = parent->children[k];
    }
    parent->children[idx] = left;
    parent->children[idx+1] = right;
    left->parent = node->parent;
    right ->parent = node->parent;
    left ->children[0] = node->children[0];
    left ->children[1] = node->children[1];
    right ->children[0] = node->children[2];
    right ->children[1] = node->children[3];
    if (left->children[0])left->children[0]->parent = left;
    if (left->children[1])  left->children[1]->parent = left;
    if (right->children[0]) right ->children[0]->parent =right;
    if (right->children[1]) right ->children[1]->parent = right;
    ++parent->nodesInserted;
    if (parent->nodesInserted > 3) {
        split(parent);
    }
    delete node;
  return val;
}

template<typename T>
int BTree<T>::search(T val) {
    Node* current = root;
    while (current) {
        int i;
        for (i =0; i< current->nodesInserted; i++) {
            if (val == current->keys[i])return i;
            if (val < current->keys[i]) {
                current = current->children[i];
                break;
            }
        }
        if (i == current->nodesInserted) {
            current = current->children[current->nodesInserted];
        }
    }
    return -1;
}
#endif //BTREE_BTREE_TPP
