#include <print>

#include "tree.hpp"

int main()
{
    Tree tree;

    TreeNode *root = tree.createTree(1);

    TreeNode *n2 = tree.insert(root, 2);
    TreeNode *n3 = tree.insert(root, 3);
    TreeNode *n4 = tree.insert(n2, 4);
    TreeNode *n5 = tree.insert(n2, 5);

    std::println("Total nodes: {}", tree.size());

    std::println("\nRoot checks:");
    std::println("Node 1: {}", tree.isRoot(*root));
    std::println("Node 2: {}", tree.isRoot(*n2));

    std::println("\nLeaf checks:");
    std::println("Node 3: {}", tree.isLeaf(*n3));
    std::println("Node 4: {}", tree.isLeaf(*n4));
    std::println("Node 2: {}", tree.isLeaf(*n2));

    std::println("\nDegrees:");
    std::println("Node 1: {}", tree.degree(*root).value());
    std::println("Node 2: {}", tree.degree(*n2).value());
    std::println("Node 5: {}", tree.degree(*n5).value());

    return 0;
}