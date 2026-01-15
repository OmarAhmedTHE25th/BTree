//
// Created by DELL on 14/01/2026.
//

#ifndef BTREE_BTREE_TPP
#define BTREE_BTREE_TPP
#include "bTree.hpp"
#include <cassert>
#include <ranges>

template<typename T>
void BTree<T>::insert(T key) {
    if (not root) {
        root = new Node(key);
        return;
    }
    if (root->nodesInserted == MAX_KEYS) {
        split(root);
    }

    Node* current = root;
    bool reachedLeaf = false;

    while (current && !reachedLeaf) {
        int j;
        for (j = 0; j < current->nodesInserted; j++) {

            if (current->children[j] && current->children[j]->nodesInserted == MAX_KEYS) {
                T promotedKey = split(current->children[j]);
                if (key < promotedKey) current = current->children[j];
                else current = current->children[j+1];

                j = -1;
                continue;
            }

            if (key < current->keys[j]) {
                if (current->children[j]) {
                    current = current->children[j];
                } else {
                    reachedLeaf = true;
                }
                break;
            }
        }

        if (!reachedLeaf && j == current->nodesInserted) {
            if (current->children[j] && current->children[j]->nodesInserted == MAX_KEYS) {
                split(current->children[j]);
                continue;
            }

            if (current->children[j]) {
                current = current->children[j];
            } else {
                reachedLeaf = true;
            }
        }
    }

    assert(current != nullptr);

    int i = current->nodesInserted - 1;
    while (i >= 0 && key < current->keys[i]) {
        current->keys[i + 1] = current->keys[i];
        --i;
    }
    current->keys[i + 1] = key;
    ++current->nodesInserted;
}

template <typename T>
int BTree<T>::getChildIdx(Node *node, Node *parent) {
    if (!node)return -1;
    int idx = 0;
    while (idx < parent->nodesInserted + 1 && parent->children[idx] != node)
        idx++;
    return idx;
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
    int idx = getChildIdx(node, parent);

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
bool BTree<T>::isLeaf(Node *node) {
    if (!node)return false;
    for (auto child : node->children) {
        if (child)return false;
    }
    return true;
}

template<typename T>
 BTree<T>::Node *BTree<T>::searchNode(T val) {
    Node* current = root;
    while (current) {
        int i;
        for (i =0; i< current->nodesInserted; i++) {
            if (val == current->keys[i])return current;
            if (val < current->keys[i]) {
                current = current->children[i];
                break;
            }
        }
        if (i == current->nodesInserted) {
            current = current->children[current->nodesInserted];
        }
    }
    return nullptr;
}

template<typename T>
 BTree<T>::Node *BTree<T>::findLeaf(T key) {
    Node* current = root;

    // We keep going until the current node has no children (is a leaf)
    while (current && current->children[0] != nullptr) {
        int i = 0;

        // Find the first key that is >= our target key
        while (i < current->nodesInserted && key >= current->keys[i]) {
            i++;
        }

        // Follow the child pointer at index 'i'
        // If key < keys[0], i is 0, we go to children[0]
        // If key > all keys, i is nodesInserted, we go to children[nodesInserted]
        current = current->children[i];
    }

    return current; // This is now your leaf node!
}

template<typename T>
bool BTree<T>::borrowable(Node *node) {
    if (!node)return false;
    if (node->nodesInserted > 1)return true;
    return false;
}

template<typename T>
void BTree<T>::remove(T key) {
    const int idx = search(key);
    if (idx==-1)return;
    Node* node = searchNode(key);
    Node* parent = node->parent;
    //case 0: node is root node
    if (node == root && node->nodesInserted == 1) {
        delete root;
        root = nullptr;
        return;
    }
    //case 1: leaf node and  > 1 key
    if (node->nodesInserted > 1 && isLeaf(node)) {
        for (int i=idx;i< node->nodesInserted-1;++i) {
            node->keys[i] = node->keys[i+1];
        }
        node->keys[MAX_KEYS - 1] = 0;
        --node->nodesInserted;
        return;
    }
    // case 2: leaf node and 1 key
    if (node->nodesInserted == 1 && isLeaf(node)) {

        bool canBorrow = false;
        const int i = getChildIdx(node,parent);

        // check left sibling
        if (i > 0) {  // there *is* a left sibling
            Node* leftSib = node->parent->children[i-1];
            if (borrowable(leftSib)) canBorrow = true;
        }

        // check right sibling (only if we haven’t found a borrowable sibling yet)
        if (!canBorrow && i < MAX_CHILDREN-1) {
            Node* rightSib = node->parent->children[i+1];
            if (borrowable(rightSib)) canBorrow = true;
        }


        //case 2a: cannot borrow from siblings.
        if (not canBorrow) {
            //case 2a.1: node is leftmost child and cannot borrow
            if (i==0) {
                Node* rightSib = parent->children[1];
                if (!rightSib)return;
                T sep = parent->keys[0];
                node->keys[1] = sep;
                parent -> keys[0] = rightSib ->keys[0];
                ++node->nodesInserted;
                parent->children[1] = nullptr;
                delete rightSib;
                remove(key);
            }
            //case 2a.2 node is not leftmost child and cannot borrow
            Node* leftSib = parent->children[i-1];
            if (!leftSib)return;
            T sep = parent->keys[i-1];
            node->keys[1] = sep;
            parent -> keys[i-1] = leftSib ->keys[0];
            ++node->nodesInserted;
            parent->children[i-1] = nullptr;
            delete leftSib;
            remove(key);

        }
        //case 2b: can borrow from siblings.
        //case 2b.1 : node is leftmost child
        if (i==0) {
            Node* rightSib = parent->children[1];
            if (!rightSib)return;
            T sep = parent->keys[0];
            node->keys[1] = sep;
            parent -> keys[0] = rightSib ->keys[0];
            ++node->nodesInserted;
            for ( int k = 0; k < rightSib->nodesInserted-1; ++k) {
                rightSib->keys[k] = rightSib->keys[k+1];
            }
            rightSib->keys[MAX_KEYS - 1] = 0;
            --rightSib->nodesInserted;
            remove(key);
        }
        //case 2b.2 : node is not leftmost
        Node* leftSib = parent->children[i-1];
        if (!leftSib)return;
        T sep = parent->keys[i-1];
        node->keys[1] = sep;
        parent -> keys[i-1] = leftSib ->keys[0];
        ++node->nodesInserted;
        for ( int k = 0; k < leftSib->nodesInserted-1; ++k) {
            leftSib->keys[k] = leftSib->keys[k+1];
        }
        leftSib->keys[MAX_KEYS - 1] = 0;
        --leftSib->nodesInserted;
        remove(key);
    }


}
#endif //BTREE_BTREE_TPP
