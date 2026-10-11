#include <print>

#include "tree.hpp"

void displayTree(Tree &tree);

int main()
{
    Tree tree;

    TreeNode *root = tree.createTree(1);

    TreeNode *n2 = tree.insert(root, 2);
    TreeNode *n3 = tree.insert(root, 3);
    TreeNode *n4 = tree.insert(root, 4);

    tree.insert(n2, 5);
    TreeNode *n6 = tree.insert(n2, 6);
    tree.insert(n3, 7);
    tree.insert(n4, 8);
    tree.insert(n6, 9);
    tree.insert(n6, 10);

    std::println("Before deletion:");
    displayTree(tree);

    std::println("\nDeleting node 2 and its descendants...");
    tree.remove(n2);

    std::println("\nAfter deletion:");
    displayTree(tree);

    std::println("\nDeleting root...");
    tree.remove(root);

    std::println("Total nodes: {}", tree.size());

    return 0;
}

void displayTree(Tree &tree)
{
    std::println("Node\tParent\tChildren");

    tree.traverseBreadthFirst([](const TreeNode &node, int value) { //
        std::print("{}\t", value);

        if (node.parent == nullptr)
            std::print("None\t");
        else
            std::print("{}\t", node.parent->data);

        if (node.children.empty())
            std::print("None");
        else
        {
            for (const TreeNode *child : node.children)
                std::print("{} ", child->data);
        }

        std::println();
    });
}
