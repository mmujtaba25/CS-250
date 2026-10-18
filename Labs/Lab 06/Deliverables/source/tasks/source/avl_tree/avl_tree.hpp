#pragma once

#include <vector>
#include <cstddef>
#include <optional>
#include <functional>

struct AVLTreeNode
{
    int data;

    AVLTreeNode *parent = nullptr;
    AVLTreeNode *left = nullptr;
    AVLTreeNode *right = nullptr;

    int height = 1;
};

class AVLTree
{
private:
    AVLTreeNode *rootNode = nullptr;
    size_t nodeCount = 0;

    void deleteSubtree(AVLTreeNode *node);
    void transplant(AVLTreeNode *node, AVLTreeNode *replacement);

    // AVL Helpers; internal thus no need for optional
    static int nodeHeight(const AVLTreeNode *node);
    static int balanceFactor(const AVLTreeNode *node);
    static void updateHeight(AVLTreeNode *node);

    AVLTreeNode *rotateRight(AVLTreeNode *node);
    AVLTreeNode *rotateLeft(AVLTreeNode *node);

    void rebalance(AVLTreeNode *node);

public:
    AVLTree();
    ~AVLTree() { clear(); }

    // delete copy and move constructors
    AVLTree(const AVLTree &) = delete;
    AVLTree &operator=(const AVLTree &) = delete;
    AVLTree(AVLTree &&) = delete;
    AVLTree &operator=(AVLTree &&) = delete;

    AVLTreeNode *createTree(int data);
    AVLTreeNode *insert(int data);

    // Queries
    const AVLTreeNode *rootContains(const AVLTreeNode &target) const;

    AVLTreeNode *search(int data);
    const AVLTreeNode *search(int data) const;

    AVLTreeNode *root() { return rootNode; }
    const AVLTreeNode *root() const { return rootNode; }

    bool isRoot(const AVLTreeNode &node) const;
    bool isLeaf(const AVLTreeNode &node) const;

    size_t countChildren(const AVLTreeNode &node) const;
    size_t size() const;

    std::optional<size_t> degree(const AVLTreeNode &node) const;
    std::optional<size_t> height(const AVLTreeNode &node) const;
    std::optional<size_t> depth(const AVLTreeNode &node) const;

    std::optional<size_t> height() const
    {
        if (rootNode == nullptr)
            return std::nullopt;

        return static_cast<size_t>(rootNode->height);
    }

    // AVL Queries
    std::optional<int> getBalanceFactor(const AVLTreeNode &node) const;

    // Traversal
    void traverseBreadthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach);
    void traversePreOrderDepthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach);
    void traversePostOrderDepthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach);
    void traverseInOrderDepthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach);

    // Deletion
    bool remove(AVLTreeNode *node);
    bool remove(int data);
    void clear();

public: // STATICS
    static const AVLTreeNode *contains(const AVLTreeNode &start, const AVLTreeNode &target);
};
