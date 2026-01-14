//
// Created by DELL on 14/01/2026.
//

#ifndef BTREE_NODE_HPP
#define BTREE_NODE_HPP
#include <array>
template<typename T>
class Node {
    Node* parent{};
    std::array<T,3> keys{};
    std::array<Node*,4> children{};
    int nodesInserted = 0;
Node() = default;
    explicit Node(T key) {
        keys[nodesInserted++] = key;
    }
    template<typename >
    friend class BTree;
};


#endif //BTREE_NODE_HPP