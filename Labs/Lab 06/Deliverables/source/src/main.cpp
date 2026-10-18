#include "avl_tree.hpp"

#include <print>
#include <iostream>
#include <string>
#include <cstdio>

void printTree(const AVLTree &tree);
void printLevel(const AVLTreeNode *node, int level, int spacing);

int main()
{
    AVLTree tree;

    std::println("Enter integers to insert into the AVL tree.");
    std::println("Enter any non-numeric character to exit.\n");

    while (true)
    {
        int value;

        std::print("Enter value: ");
        std::fflush(stdout);

        if (!(std::cin >> value))
            break;

        tree.insert(value);

        std::println();
        printTree(tree);

        std::println();
    }

    return 0;
}

// MARK: Helpers

void printLevel(const AVLTreeNode *node, int level, int spacing)
{
    if (level == 0)
    {
        std::print("{:^{}}", node ? std::to_string(node->data) : " ", spacing);
        return;
    }

    // recursively print each levvel
    printLevel(node ? node->left : nullptr, level - 1, spacing);
    printLevel(node ? node->right : nullptr, level - 1, spacing);
}

void printTree(const AVLTree &tree)
{
    const AVLTreeNode *root = tree.root();

    if (root == nullptr)
        return;

    int height = root->height;

    for (int level = 0; level < height; level++)
    {
        int spacing = 4 * (1 << (height - level - 1));

        printLevel(root, level, spacing);

        std::println();
    }
}
