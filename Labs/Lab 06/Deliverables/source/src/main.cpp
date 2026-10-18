#include "avl_tree.hpp"

#include <print>
#include <format>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>

std::string nodeLabel(const AVLTreeNode *node);
size_t maxLabelWidth(const AVLTreeNode *node);

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

std::string nodeLabel(const AVLTreeNode *node)
{
    if (node == nullptr)
        return "";

    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right ? node->right->height : 0;

    int balance = leftHeight - rightHeight;

    return std::format("{}:[{} {:+}]", node->data, node->height - 1, balance);
}

size_t maxLabelWidth(const AVLTreeNode *node)
{
    if (node == nullptr)
        return 1;

    return std::max({nodeLabel(node).size(), maxLabelWidth(node->left), maxLabelWidth(node->right)});
}

void printLevel(const AVLTreeNode *node, int level, int spacing)
{
    if (level == 0)
    {
        std::print("{:^{}}", nodeLabel(node), spacing);
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
    {
        std::println("(empty)");
        return;
    }

    int height = root->height;
    int width = static_cast<int>(maxLabelWidth(root)) + 2;

    for (int level = 0; level < height; level++)
    {
        int spacing = width * (1 << (height - level - 1));

        printLevel(root, level, spacing);

        std::println();
    }
}
