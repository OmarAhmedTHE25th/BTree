#include <iostream>

#include "bTree.hpp"

int main() {
    BTree<int> tree;
    tree.insert(10);
    tree.insert(90);
    tree.insert(30);
    std::array<int,5> array{};
    array[0] = 10;
    array[1] = 20;
    std::cout << array.size();
    return 0;
}
