//
// Created by DELL on 14/01/2026.
//

#ifndef BTREE_BTREE_TPP
#define BTREE_BTREE_TPP
#include "bTree.hpp"
#include <cassert>

template<typename T>
void BTree<T>::insert(T key) {
    if (not root) {
        root = new Node(key);
        return;
    }

    // Safety: If the root itself is full, split it before doing anything else!
    if (root->nodesInserted == MAX_KEYS) {
        split(root);
    }

    Node* current = root;
    bool reachedLeaf = false;

    while (current && !reachedLeaf) {
        int j;
        for (j = 0; j < current->nodesInserted; j++) {
            // Check if we need to split a full child as we go down
            if (current->children[j] && current->children[j]->nodesInserted == MAX_KEYS) {
                T promotedKey = split(current->children[j]);
                // Re-evaluate which way to go after the split
                if (key < promotedKey) current = current->children[j];
                else current = current->children[j+1];

                // Restart the loop for this node because the keys changed
                j = -1;
                continue;
            }

            if (key < current->keys[j]) {
                if (current->children[j]) {
                    current = current->children[j];
                } else {
                    reachedLeaf = true; // Stay on this node, it's a leaf!
                }
                break;
            }
        }

        // If we checked all keys and it's bigger than all of them
        if (!reachedLeaf && j == current->nodesInserted) {
            if (current->children[j] && current->children[j]->nodesInserted == MAX_KEYS) {
                split(current->children[j]);
                // After split, 'current' stays the same, loop restarts to pick right child
                continue;
            }

            if (current->children[j]) {
                current = current->children[j];
            } else {
                reachedLeaf = true; // It's a leaf!
            }
        }
    }

    // Now current is GUARANTEED to be a valid leaf node pointer
    assert(current != nullptr);

    // Standard insertion into the leaf node
    int i = current->nodesInserted - 1;
    while (i >= 0 && key < current->keys[i]) {
        current->keys[i + 1] = current->keys[i];
        --i;
    }
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
        assert(root->children[0] != nullptr && root->children[1] != nullptr);
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
    while (idx < parent->nodesInserted + 1 && parent->children[idx] != node)
        idx++;

    assert(idx <= parent->nodesInserted);
    const int childCount = parent->nodesInserted +1;
    for (int k = childCount-1;k > idx; --k) {
        assert(k+1 < MAX_CHILDREN);
        parent->children[k+1] = parent->children[k];
    }
    parent->children[idx] = left;
    parent->children[idx+1] = right;
    assert(left != nullptr && right != nullptr);
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
    assert(left->parent == parent && right->parent == parent);

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

template<typename T>
void BTree<T>::remove(T key) {
    //case 1: leaf node > 1 key

}
#endif //BTREE_BTREE_TPP
