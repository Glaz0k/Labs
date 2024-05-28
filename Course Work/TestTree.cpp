#include "TestTree.hpp"
#include "RedBlackTree.hpp"
#include <algorithm>

void outputTree(RedBlackTree<int, bool>& tree, std::ostream& out)
{
    for (const auto& p : tree)
    {
        out << p.first << ' ';
    }
}

void testTree(std::ostream& out)
{
    out << "\nStarted tree test\n";

    RedBlackTree<int, bool> test;
    test[1];
    test[2];
    test[3];
    test[4];
    test[5];
    test[6];
    test[7];
    test[8];
    test[9];
    test[10];

    test.erase(5);
    test[5];
    test[11];
    test[12];

    test.erase(10);
    test.erase(3);
    test.clear();

    RedBlackTree<int, bool> emptyT;
    RedBlackTree<int, bool> tree;
    // 0:                       0
    // 1:          -4                       4
    // 2:    -6          -2           2           6
    // 3: -7    -5    -3    -1     1     3     5     7

    out << "Insert:\n";
    int insert[] = { 0, -4, 4, -6, -2, 2, 6, -7, -5, -3, -1, 1, 3, 5, 7, -2, 6 };
    for (int n : insert)
    {
        out << tree.insert({n, true}).second;
    }
    out << '\n';
    outputTree(tree, out);
    out << '\n';
    out << "count: " << tree.size() << '\n';

    out << "\nSearch:\n";
    int search[] = { -1, -6, 12, 0, -15, 1 };
    for (int n : search)
    {
        out << "search " << n << ": " << (tree.find(n) != tree.end()) << '\n';
    }

    out << "\nDelete:\n";
    int erase[] = { -1, -7, -6, -2, -4, 4, -2, 12, 0, -13, -4, -1 };
    for (int i = 0; i < 12; ++i)
    {
        out << "delete " << erase[i] << ": " << (tree.erase(erase[i]) != tree.end()) << '\n';
        outputTree(tree, out);
        out << '\n';
        out << "count: " << tree.size() << '\n';
    }
    out << "delete from empty 26: " << (emptyT.erase(26) != tree.end()) << '\n';
    outputTree(emptyT, out);
    out << '\n';
    out << "count: " << emptyT.size() << '\n';

    out << "\nMove:\n";
    RedBlackTree<int, bool> moveTree = std::move(tree);
    outputTree(moveTree, out);
    out << '\n';
    tree = std::move(moveTree);
    outputTree(tree, out);
    out << '\n';

    out << "\nEnded tree test\n";
}
