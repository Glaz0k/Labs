#include <iostream>
#include "BinarySearchTree.h"

void testTree()
{
    BinarySearchTree<int> emptyT;
    BinarySearchTree<int> tree;
    // 0:                       0
    // 1:          -4                       4
    // 2:    -6          -2           2           6
    // 3: -7    -5    -3    -1     1     3     5     7

    std::cout << "Insert:\n";
    int insert[] = { 0, -4, 4, -6, -2, 2, 6, -7, -5, -3, -1, 1, 3, 5, 7, -2, 6 };
    for (int n : insert)
    {
        std::cout << tree.insertNode(n);
    }
    std::cout << '\n';
    tree.output(std::cout);
    std::cout << '\n';
    std::cout << "count: " << tree.getNumberOfNodes() << '\n';
    std::cout << "height: " << tree.getHeight() << '\n';

    std::cout << "\nSearch:\n";
    int search[] = { -1, -6, 12, 0, -15, 1 };
    for (int n : search)
    {
        std::cout << "search " << n << ": " << tree.searchKeyIterative(n) << '\n';
    }

    std::cout << "\nDelete:\n";
    int erase[] = { -1, -7, -6, -2, -4, 4, -2, 12, 0, -13, -4, -1 };
    for (int i = 0; i < 12; ++i)
    {
        std::cout << "delete " << erase[i] << ": " << tree.deleteNode(erase[i]) << '\n';
        tree.output(std::cout);
        std::cout << '\n';
        std::cout << "count: " << tree.getNumberOfNodes() << '\n';
        std::cout << "height: " << tree.getHeight() << '\n';
    }
    std::cout << "delete from empty 26: " << emptyT.deleteNode(26) << '\n';
    emptyT.output(std::cout);
    std::cout << '\n';
    std::cout << "count: " << emptyT.getNumberOfNodes() << '\n';
    std::cout << "height: " << emptyT.getHeight() << '\n';

    std::cout << "\nInorder Walk:\n";
    std::cout << "recursive:\n";
    tree.inorderWalk(std::cout);
    std::cout << '\n';
    emptyT.inorderWalk(std::cout);
    std::cout << '\n';
    std::cout << "iterative:\n";
    tree.inorderWalkIterative(std::cout);
    std::cout << '\n';
    emptyT.inorderWalkIterative(std::cout);
    std::cout << '\n';
    std::cout << "by levels:\n";
    tree.walkByLevels(std::cout);
    std::cout << '\n';
    emptyT.walkByLevels(std::cout);
    std::cout << '\n';

    std::cout << "\nMove:\n";
    BinarySearchTree<int> moveTree = std::move(tree);
    moveTree.output(std::cout);
    std::cout << '\n';
    tree = std::move(moveTree);
    tree.output(std::cout);
    std::cout << '\n';

    BinarySearchTree<int> tree1;
    BinarySearchTree<int> tree2;
    for (int n : insert)
    {
        tree1.insertNode(n);
        tree2.insertNode(n);
    }

    std::cout << "\nSimilar:\n";
    std::cout << "similar: " << tree1.isSimilar(emptyT) << '\n';
    std::cout << "similar: " << tree1.isSimilar(tree2) << '\n';
    tree2.insertNode(12);
    std::cout << "similar: " << tree1.isSimilar(tree2) << '\n';
    tree2.deleteNode(-7);
    std::cout << "similar: " << tree1.isSimilar(tree2) << '\n';
    tree2.insertNode(-7);
    tree2.deleteNode(12);
    std::cout << "similar: " << tree1.isSimilar(tree2) << '\n';

    std::cout << "\nIdentical Key:\n";
    std::cout << "identical: " << tree1.isIdenticalKey(emptyT) << '\n';
    std::cout << "identical: " << tree1.isIdenticalKey(tree2) << '\n';
    emptyT.insertNode(0);
    std::cout << "identical: " << tree1.isIdenticalKey(emptyT) << '\n';
    emptyT.insertNode(13);
    std::cout << "identical: " << tree1.isIdenticalKey(emptyT) << '\n';
    emptyT.deleteNode(0);
    std::cout << "identical: " << tree1.isIdenticalKey(emptyT) << '\n';
}

int main()
{
    testTree();

    return 0;
}