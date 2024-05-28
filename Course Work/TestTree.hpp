#ifndef TEST_TREE_HPP
#define TEST_TREE_HPP

#include <ostream>
#include "RedBlackTree.hpp"

void outputTree(RedBlackTree<int, bool>& tree, std::ostream& out);
void testTree(std::ostream& out);

#endif