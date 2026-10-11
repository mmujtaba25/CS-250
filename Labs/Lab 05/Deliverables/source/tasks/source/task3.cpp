#include <print>

#include "tree.hpp"

int main()
{
    Tree tree;

    TreeNode *root = tree.createTree(1);

    TreeNode *n2 = tree.insert(root, 2);
    TreeNode *n3 = tree.insert(root, 3);
    TreeNode *n4 = tree.insert(n2, 4);
    TreeNode *n5 = tree.insert(n4, 5);
    TreeNode *n6 = tree.insert(n3, 6);

    std::println("Node\tHeight\tDepth");

    for (const TreeNode *node : {root, n2, n3, n4, n5, n6})
    {
        std::println("{}\t{}\t{}", node->data, tree.height(*node).value(), tree.depth(*node).value());
    }

    tree.clear();

    return 0;
}
