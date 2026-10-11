#include <print>

#include "tree.hpp"

int main()
{
    Tree tree;

    TreeNode *root = tree.createTree(1);

    TreeNode *n2 = tree.insert(root, 2);
    TreeNode *n3 = tree.insert(root, 3);
    TreeNode *n4 = tree.insert(root, 4);

    tree.insert(n2, 5);
    tree.insert(n2, 6);
    tree.insert(n3, 7);
    tree.insert(n4, 8);
    tree.insert(n4, 9);

    std::print("BFS:        ");

    tree.traverseBreadthFirst([](const TreeNode &node, int value) { //
        std::print("{} ", value);
    });

    std::println();

    std::print("Pre-order:  ");

    tree.traversePreOrderDepthFirst([](const TreeNode &node, int value) { //
        std::print("{} ", value);
    });

    std::println();

    std::print("Post-order: ");

    tree.traversePostOrderDepthFirst([](const TreeNode &node, int value) { //
        std::print("{} ", value);
    });

    std::println();

    return 0;
}
