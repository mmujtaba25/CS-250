#pragma once

#include <functional>

struct DoubleSidedNode
{
    int data;
    DoubleSidedNode *next;
    DoubleSidedNode *previous;
};

using DoubleSidedNodeLink = DoubleSidedNode *&;
using DoubleSidedConstNode = const DoubleSidedNode *;


class DoublyLinkedList
{
public:
    DoublyLinkedList() : head(nullptr), count_(0) {}
    DoublyLinkedList(int value) : head(new DoubleSidedNode{value, nullptr, nullptr}), count_(1) {}
    ~DoublyLinkedList() { removeAll(); }

    // copy
    DoublyLinkedList(const DoublyLinkedList &other);
    DoublyLinkedList &operator=(const DoublyLinkedList &other);

    // move
    DoublyLinkedList(DoublyLinkedList &&old) noexcept;
    DoublyLinkedList &operator=(DoublyLinkedList &&old) noexcept;

    // Get

    int *get(size_t index, size_t *nodes_examined = nullptr);
    inline int *getFirst(size_t *nodes_examined = nullptr) { return get(0, nodes_examined); }
    inline int *getLast(size_t *nodes_examined = nullptr) { return get(count() - 1, nodes_examined); }

    // Insert

    bool insertAt(int value, size_t index);
    inline bool insertStart(int value) { return insertAt(value, 0); }
    inline bool insertEnd(int value) { return insertAt(value, count()); }

    // Remove

    bool removeIf(const std::function<bool(const int &, size_t)> &predicate);

    bool removeAt(int index);
    inline bool removeFirst() { return removeAt(0); }
    inline bool removeLast() { return removeAt(count() - 1); }

    void removeAll();

    // Count

    inline size_t count() const { return count_; }

    // For Each

    void forEach(std::function<bool(DoubleSidedNodeLink, size_t)> func);
    void forEach(std::function<bool(DoubleSidedConstNode, size_t)> func) const;

private:
    DoubleSidedNode *head = nullptr;
    int count_ = 0;

    inline const bool indexInBounds(size_t i, bool includeLast = false) const
    {
        return i >= 0 && (includeLast ? i <= count_ : i < count_);
    }
};