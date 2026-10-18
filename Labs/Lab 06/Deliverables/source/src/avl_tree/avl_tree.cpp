#include "avl_tree.hpp"

#include <cstddef>
#include <optional>
#include <algorithm>
#include <memory>
#include <queue>
#include <stack>

// MARK: Helpers

AVLTree::AVLTree() {}

AVLTreeNode *AVLTree::createTree(int data)
{
    clear();

    rootNode = new AVLTreeNode{
        .data = data,
        .parent = nullptr,
        .left = nullptr,
        .right = nullptr,
        .height = 0 //
    };

    nodeCount = 1;

    return rootNode;
}

AVLTreeNode *AVLTree::insert(int data)
{
    // Empty tree; the inserted node becomes the root
    if (rootNode == nullptr)
        return createTree(data);

    // find proper parent node for given value
    AVLTreeNode *current = rootNode;
    AVLTreeNode *parent = nullptr;
    while (current != nullptr)
    {
        parent = current;

        // if less than current, move left
        if (data < current->data)
            current = current->left;
        // if greater then current, move right
        else if (data > current->data)
            current = current->right;
        // if equal match found, just return it
        else
            return current;
    }

    // create unattached node
    AVLTreeNode *newNode = new AVLTreeNode{
        .data = data,
        .parent = parent,
        .left = nullptr,
        .right = nullptr,
        .height = 0 //
    };

    // insert left if less than found parent
    if (data < parent->data)
        parent->left = newNode;
    // insert right if greater than or equal found parent
    else
        parent->right = newNode;

    nodeCount++;

    // rebalance entire tree
    rebalance(parent);

    return newNode;
}

// MARK: AVL Helpers

int AVLTree::nodeHeight(const AVLTreeNode *node)
{
    if (node == nullptr)
        return 0;

    return node->height;
}

int AVLTree::balanceFactor(const AVLTreeNode *node)
{
    if (node == nullptr)
        return 0;

    // Positive: left subtree is taller
    // Negative: right subtree is taller
    return nodeHeight(node->left) - nodeHeight(node->right);
}

void AVLTree::updateHeight(AVLTreeNode *node)
{
    if (node == nullptr)
        return;

    node->height = 1 + std::max(nodeHeight(node->left), nodeHeight(node->right));
}

AVLTreeNode *AVLTree::rotateLeft(AVLTreeNode *node)
{
    if (node == nullptr || node->right == nullptr)
        return node;

    AVLTreeNode *newRoot = node->right;
    AVLTreeNode *subtree = newRoot->left;
    AVLTreeNode *parent = node->parent;

    // attach the new root to the original parent
    newRoot->parent = parent;

    // relocating root node, change the ref for that
    if (parent == nullptr)
        this->rootNode = newRoot;
    // no root node; update parent's left ref
    else if (parent->left == node)
        parent->left = newRoot;
    else
        parent->right = newRoot;

    // move the original node to the left
    newRoot->left = node;
    node->parent = newRoot;

    // rransfer the new root's left subtree
    node->right = subtree;

    if (subtree != nullptr)
        subtree->parent = node;

    // update heights
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

AVLTreeNode *AVLTree::rotateRight(AVLTreeNode *node)
{
    if (node == nullptr || node->left == nullptr)
        return node;

    AVLTreeNode *newRoot = node->left;
    AVLTreeNode *subtree = newRoot->right;
    AVLTreeNode *parent = node->parent;

    // attach the new root to the original parent
    newRoot->parent = parent;

    // relocating root node, update local ref
    if (parent == nullptr)
        this->rootNode = newRoot;
    // no root node; update parent's left ref
    else if (parent->left == node)
        parent->left = newRoot;
    else
        parent->right = newRoot;

    // move the original node to the right
    newRoot->right = node;
    node->parent = newRoot;

    // rransfer the new root's right subtree
    node->left = subtree;

    if (subtree != nullptr)
        subtree->parent = node;

    // update heights
    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

void AVLTree::rebalance(AVLTreeNode *node)
{
    // traverse upward, updating heights and correcting imbalances
    while (node != nullptr)
    {
        updateHeight(node);

        // check and update balance

        int balance = balanceFactor(node);

        // left-heavy subtree
        if (balance > 1)
        {
            // LR: first rotate the left child
            if (balanceFactor(node->left) < 0)
                rotateLeft(node->left);

            // LL: or second rotation of LR
            node = rotateRight(node);
        }

        // right-heavy subtree
        else if (balance < -1)
        {
            // RL: first rotate the right child
            if (balanceFactor(node->right) > 0)
                rotateRight(node->right);

            // RR: or second rotation of RL
            node = rotateLeft(node);
        }

        // traver upwards
        node = node->parent;
    }
}

// MARK: Queries

const AVLTreeNode *AVLTree::rootContains(const AVLTreeNode &target) const
{
    if (rootNode == nullptr)
        return nullptr;

    return contains(*rootNode, target);
}

AVLTreeNode *AVLTree::search(int data)
{
    AVLTreeNode *current = rootNode;

    // simple binary search
    while (current != nullptr)
    {
        if (data == current->data)
            return current;

        if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    return nullptr;
}

const AVLTreeNode *AVLTree::search(int data) const
{
    const AVLTreeNode *current = rootNode;

    // simple binary search
    while (current != nullptr)
    {
        if (data == current->data)
            return current;

        if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    return nullptr;
}

bool AVLTree::isRoot(const AVLTreeNode &node) const
{
    return rootNode == &node;
}

bool AVLTree::isLeaf(const AVLTreeNode &node) const
{
    return rootContains(node) &&
           node.left == nullptr &&
           node.right == nullptr;
}

size_t AVLTree::countChildren(const AVLTreeNode &node) const
{
    size_t count = 1;

    // recursively add left and right counts
    if (node.left != nullptr)
        count += countChildren(*node.left);
    if (node.right != nullptr)
        count += countChildren(*node.right);

    // countChildren called once for each node recursively
    // count = 1 even if no children
    return count;
}

size_t AVLTree::size() const
{
    return nodeCount;
}

std::optional<size_t> AVLTree::degree(const AVLTreeNode &node) const
{
    if (!rootContains(node))
        return std::nullopt;

    size_t count = 0;

    if (node.left != nullptr)
        count++;

    if (node.right != nullptr)
        count++;

    return count;
}

std::optional<size_t> AVLTree::height(const AVLTreeNode &node) const
{
    if (!rootContains(node))
        return std::nullopt;

    return static_cast<size_t>(node.height);
}

std::optional<size_t> AVLTree::depth(const AVLTreeNode &node) const
{
    if (!rootContains(node))
        return std::nullopt;

    size_t depth = 0;
    const AVLTreeNode *current = &node;

    while (current != rootNode)
    {
        depth++;
        current = current->parent;
    }

    return depth;
}

std::optional<int> AVLTree::getBalanceFactor(const AVLTreeNode &node) const
{
    if (!rootContains(node))
        return std::nullopt;

    return balanceFactor(&node);
}

// MARK: Traverse

void AVLTree::traverseBreadthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach)
{
    if (rootNode == nullptr)
        return;

    std::queue<const AVLTreeNode *> nodes;
    nodes.push(rootNode);

    while (!nodes.empty())
    {
        // give the front node to consumer
        const AVLTreeNode *current = nodes.front();
        forEach(*current, current->data);
        nodes.pop(); // remove; since read

        // add children of current node to the back if any
        // if children present, loop continues
        if (current->left != nullptr)
            nodes.push(current->left);

        if (current->right != nullptr)
            nodes.push(current->right);
    }
}

void AVLTree::traversePreOrderDepthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach)
{
    if (rootNode == nullptr)
        return;

    auto traverse = [&](auto &&self, const AVLTreeNode *node) -> void
    {
        if (node == nullptr)
            return; // base condition

        // visit parent before children
        forEach(*node, node->data);

        // recursively visit each child from left to right
        self(self, node->left);
        self(self, node->right);
    };

    traverse(traverse, rootNode);
}

void AVLTree::traversePostOrderDepthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach)
{
    if (rootNode == nullptr)
        return;

    auto traverse = [&](auto &&self, const AVLTreeNode *node) -> void
    {
        if (node == nullptr)
            return; // base condition

        // recursively visit each child from left to right
        self(self, node->left);
        self(self, node->right);

        // visit parent after all children
        forEach(*node, node->data);
    };

    traverse(traverse, rootNode);
}

void AVLTree::traverseInOrderDepthFirst(std::function<void(const AVLTreeNode &node, int value)> forEach)
{
    if (rootNode == nullptr)
        return;

    auto traverse = [&](auto &&self, const AVLTreeNode *node) -> void
    {
        if (node == nullptr)
            return; // base condition

        // visit left subtree
        self(self, node->left);

        // visit parent
        forEach(*node, node->data);

        // visit right subtree
        self(self, node->right);
    };

    traverse(traverse, rootNode);
}

// MARK: Deletion

bool AVLTree::remove(AVLTreeNode *node)
{
    if (node == nullptr || rootNode == nullptr)
        return false;

    if (!rootContains(*node))
        return false;

    AVLTreeNode *rebalanceFrom = nullptr;

    // node has no left child
    if (node->left == nullptr)
    {
        rebalanceFrom = node->parent;

        // replace the deleted node with right child if any
        AVLTreeNode *replacement = node->right;
        transplant(node, replacement);

        // update where to rebalance from
        if (rebalanceFrom == nullptr)
            rebalanceFrom = replacement;
    }
    // node has no right child
    else if (node->right == nullptr)
    {
        rebalanceFrom = node->parent;

        // replace the deleted node with left child if any
        AVLTreeNode *replacement = node->left;
        transplant(node, replacement);

        // update where to rebalance from
        if (rebalanceFrom == nullptr)
            rebalanceFrom = replacement;
    }
    // node has both children
    else
    {
        // find the inorder successor; the leftmost node in the right subtree.
        AVLTreeNode *successor = node->right;
        while (successor->left != nullptr)
            successor = successor->left;

        // if the successor is not the immediate right child, detach it from its original position before relocating it.
        if (successor->parent != node)
        {
            rebalanceFrom = successor->parent;

            // remove the successor from its original position
            transplant(successor, successor->right);

            // attach the deleted node's right subtree
            successor->right = node->right;
            successor->right->parent = successor;
        }
        else
        {
            rebalanceFrom = successor;
        }

        // replace the deleted node with its successor
        transplant(node, successor);

        successor->left = node->left;
        successor->left->parent = successor;
        successor->height = node->height;
    }

    delete node;
    nodeCount--;

    rebalance(rebalanceFrom);

    return true;
}

bool AVLTree::remove(int data)
{
    AVLTreeNode *node = search(data);

    // not found
    if (node == nullptr)
        return false;

    return remove(node);
}

void AVLTree::clear()
{
    deleteSubtree(rootNode);

    // clear references
    rootNode = nullptr;
    nodeCount = 0;
}

// MARK: STATICS

const AVLTreeNode *AVLTree::contains(const AVLTreeNode &start, const AVLTreeNode &target)
{
    if (&start == &target)
    {
        return &start;
    }

    // recursively check left subtree
    if (start.left != nullptr)
    {
        const AVLTreeNode *node = contains(*start.left, target);

        if (node != nullptr)
            return node; // only return if found, else continue
    }
    // recursively check left subtree
    if (start.right != nullptr)
    {
        const AVLTreeNode *node = contains(*start.right, target);

        if (node != nullptr)
            return node;
    }

    return nullptr;
}

// MARK: Privates

void AVLTree::transplant(AVLTreeNode *node, AVLTreeNode *replacement)
{
    if (node->parent == nullptr)
        rootNode = replacement;

    else if (node->parent->left == node)
        node->parent->left = replacement;

    else
        node->parent->right = replacement;

    if (replacement != nullptr)
        replacement->parent = node->parent;
}

void AVLTree::deleteSubtree(AVLTreeNode *node)
{
    if (node == nullptr)
        return;

    deleteSubtree(node->left);
    deleteSubtree(node->right);

    delete node;
}
