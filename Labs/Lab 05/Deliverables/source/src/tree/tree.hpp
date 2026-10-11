#pragma once

#include <vector>
#include <cstddef>
#include <optional>
#include <functional>

struct TreeNode
{
    int data;
    TreeNode *parent;
    std::vector<TreeNode *> children;
};

class Tree
{
private:
    TreeNode rootNode{};
    bool hasRoot = false;

    void deleteSubtree(TreeNode *node);

public:
    Tree();
    ~Tree() { clear(); }

    // delete copy and move constructors
    Tree(const Tree &) = delete;
    Tree &operator=(const Tree &) = delete;
    Tree(Tree &&) = delete;
    Tree &operator=(Tree &&) = delete;

    TreeNode *createTree(int data);
    TreeNode *insert(TreeNode *parent, int data);

    // Queries
    const TreeNode *rootContains(const TreeNode &target) const;

    bool isRoot(const TreeNode &node) const;
    bool isLeaf(const TreeNode &node) const;

    size_t countChildren(const TreeNode &node) const;
    size_t size() const;
    std::optional<size_t> degree(const TreeNode &node) const;
    std::optional<size_t> height(const TreeNode &node) const;
    std::optional<size_t> depth(const TreeNode &node) const;

    std::optional<size_t> height() const { return height(rootNode); }

    // Traversal
    void traverseBreadthFirst(std::function<void(const TreeNode &node, int value)> forEach);
    void traversePreOrderDepthFirst(std::function<void(const TreeNode &node, int value)> forEach);
    void traversePostOrderDepthFirst(std::function<void(const TreeNode &node, int value)> forEach);

    // Deletion
    bool remove(TreeNode *node);
    void clear() { remove(&rootNode); }

public: // STATICS
    static const TreeNode *contains(const TreeNode &start, const TreeNode &target);
};
