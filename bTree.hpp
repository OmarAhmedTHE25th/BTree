//
// Created by DELL on 14/01/2026.
//

#ifndef BTREE_BTREE_HPP
#define BTREE_BTREE_HPP
#include "node.hpp"

template<typename T>
class BTree {
    using Node = Node<T>;
    Node root{};
public:
    BTree()=default;
   void insert(T key);
   void remove(T key);
};

#include "bTree.tpp"
#endif //BTREE_BTREE_HPP