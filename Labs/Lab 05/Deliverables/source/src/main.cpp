#include <print>

#include "tree.hpp"

void analyzeTree(Tree &tree);

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
    tree.insert(n4, 9);
    tree.insert(n6, 10);
    tree.insert(n6, 11);

    std::println("Before Deletion");
    analyzeTree(tree);

    tree.remove(n2);

    std::println("\nAfter Deletion");
    analyzeTree(tree);

    return 0;
}

void analyzeTree(Tree &tree)
{
    size_t leafCount = 0;

    tree.traverseBreadthFirst([&](const TreeNode &node, int value) { //
        if (node.children.empty())
            leafCount++;
    });

    std::println("> Total Nodes: {}", tree.size());
    std::println("> Leaf Nodes: {}", leafCount);

    if (tree.height().has_value())
        std::println("> Height: {}", tree.height().value());
    else
        std::println("> No Tree Height; Tree Empty");

    std::print("> BFS: ");
    tree.traverseBreadthFirst([](const TreeNode &node, int value) { //
        std::print("{} ", value);
    });

    std::print("\n> Pre-order DFS: ");
    tree.traversePreOrderDepthFirst([](const TreeNode &node, int value) { //
        std::print("{} ", value);
    });

    std::print("\n> Post-order DFS: ");
    tree.traversePostOrderDepthFirst([](const TreeNode &node, int value) { //
        std::print("{} ", value);
    });

    std::println();
}
