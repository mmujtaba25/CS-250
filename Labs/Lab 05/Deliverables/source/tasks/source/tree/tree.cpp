#include "tree.hpp"

#include <cstddef>
#include <optional>
#include <algorithm>
#include <memory>
#include <queue>
#include <stack>

// MARK: Helpers

Tree::Tree() {}

TreeNode *Tree::createTree(int data)
{
    clear();

    rootNode = TreeNode{
        .data = data,
        .parent = nullptr,
        .children = {} //
    };

    hasRoot = true;
    return &rootNode;
}

TreeNode *Tree::insert(TreeNode *parent, int data)
{
    if (parent == nullptr)
        return nullptr;

    if (!rootContains(*parent))
        return nullptr;

    TreeNode *newNode = new TreeNode{
        .data = data,
        .parent = parent,
        .children = {} //
    };

    parent->children.push_back(newNode);

    return newNode;
}

// MARK: Queries

const TreeNode *Tree::rootContains(const TreeNode &target) const
{
    if (!hasRoot)
        return nullptr;

    return contains(rootNode, target);
}

bool Tree::isRoot(const TreeNode &node) const
{
    return hasRoot && &node == &rootNode;
}

bool Tree::isLeaf(const TreeNode &node) const
{
    return rootContains(node) && node.children.empty();
}

size_t Tree::countChildren(const TreeNode &node) const
{
    size_t count = 1;

    for (const TreeNode *child : node.children)
        count += countChildren(*child);

    // countChildren called once for each node recursively
    // count = 1 even if no children
    return count;
}

size_t Tree::size() const
{
    if (!hasRoot)
        return 0;

    return countChildren(rootNode);
}

std::optional<size_t> Tree::degree(const TreeNode &node) const
{
    if (!rootContains(node))
        return std::nullopt;

    return node.children.size();
}

std::optional<size_t> Tree::height(const TreeNode &node) const
{
    if (!rootContains(node))
        return std::nullopt;

    // lambda to recursively calculate height
    auto calculateHeight = [&](auto &&self, const TreeNode &current) -> size_t
    {
        size_t max_height = 0;

        // this acts as base confition; loop not called when children empty, in that case max_height++
        for (const TreeNode *child : current.children)
        {
            // recursively calculate height of the children
            size_t child_height = 1 + self(self, *child);
            max_height = std::max(max_height, child_height);
        }

        return max_height;
    };

    return calculateHeight(calculateHeight, node);
}

std::optional<size_t> Tree::depth(const TreeNode &node) const
{
    if (!rootContains(node))
        return std::nullopt;

    size_t depth = 0;
    const TreeNode *current = &node;

    while (current != &rootNode)
    {
        depth++;
        current = current->parent;
    }

    return depth;
}

// MARK: Traverse

void Tree::traverseBreadthFirst(std::function<void(const TreeNode &node, int value)> forEach)
{
    if (!hasRoot)
        return;

    std::queue<const TreeNode *> nodes;
    nodes.push(&rootNode);

    while (!nodes.empty())
    {
        // give the front node to consumer
        const TreeNode *current = nodes.front();
        forEach(*current, current->data);
        nodes.pop(); // remove; since read

        // add children of current node to the back
        // if any, if children present, loop continues
        for (const TreeNode *child : current->children)
            nodes.push(child);
    }
}

void Tree::traversePreOrderDepthFirst(std::function<void(const TreeNode &node, int value)> forEach)
{
    if (!hasRoot)
        return;

    std::stack<const TreeNode *> nodes;
    nodes.push(&rootNode);

    while (!nodes.empty())
    {
        // give the top node to consumer
        const TreeNode *current = nodes.top();
        forEach(*current, current->data);
        nodes.pop(); // remove; since read

        // Push children in reverse order
        // reverse since stack is LIFO
        for (size_t i = current->children.size(); i > 0; i--)
            nodes.push(current->children[i - 1]);
    }
}

void Tree::traversePostOrderDepthFirst(std::function<void(const TreeNode &node, int value)> forEach)
{
    if (!hasRoot)
        return;

    std::stack<const TreeNode *> nodes;
    std::stack<const TreeNode *> reversed;

    nodes.push(&rootNode);

    // fill the reversed stack
    // first stack processes parents before children, from right to left
    while (!nodes.empty())
    {
        // store for traversal later
        // since stack is LIFO, rightmost child is stored first
        const TreeNode *current = nodes.top();
        reversed.push(current);
        nodes.pop(); // remove; since read

        // Push children from left to right
        for (const TreeNode *child : current->children)
            nodes.push(child);
    }

    while (!reversed.empty())
    {
        // give the top node to consumer
        const TreeNode *current = reversed.top();
        forEach(*current, current->data);
        reversed.pop(); // remove; since read
    }
}

// MARK: Deletion

bool Tree::remove(TreeNode *node)
{
    if (node == nullptr || !hasRoot)
        return false;

    if (!rootContains(*node))
        return false;

    // Deleting the root clears the entire tree
    if (node == &rootNode)
    {
        for (TreeNode *child : rootNode.children)
            deleteSubtree(child);

        rootNode = TreeNode{};
        hasRoot = false;

        return true;
    }

    // Remove the node from its parent's children
    TreeNode *parent = node->parent;
    auto &children = parent->children;

    auto it = std::find(children.begin(), children.end(), node);

    if (it == children.end())
        return false;

    children.erase(it);

    // Delete the node and all its descendants
    deleteSubtree(node);

    return true;
}

// MARK: STATICS

const TreeNode *Tree::contains(const TreeNode &start, const TreeNode &target)
{
    if (&start == &target)
    {
        return &start;
    }

    // recursively check children elements
    for (const TreeNode *child : start.children)
    {
        const TreeNode *node = contains(*child, target);
        if (node != nullptr)
            return node; // only return if found, else continue
    }

    return nullptr;
}

// MARK: Privates

void Tree::deleteSubtree(TreeNode *node)
{
    for (TreeNode *child : node->children)
        deleteSubtree(child);

    delete node;
}
