#include "TestTree.hpp"
#include "RedBlackTree.hpp"

void testTree(std::ostream& out)
{
    RedBlackTree< size_t, bool > test;
    test[1];
    test[2];
    test[3];
    test[4];
    test[5];
    test[6];
    test[7];
    test[8];

    test.clear();
}
