#include <iostream>

#include "bTree.hpp"

int main() {
    BTree<int> tree;
    tree.insert(10);
    tree.insert(90);
    tree.insert(30);

    return 0;
}
