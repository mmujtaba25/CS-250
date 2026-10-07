#pragma once

#include <functional>

#include "node.hpp"

class LinkedList
{
public:
    LinkedList() : head(nullptr), count_(0) {}
    LinkedList(int value) : head(new Node{value, nullptr}), count_(1) {}
    ~LinkedList() { removeAll(); }

    // copy
    LinkedList(const LinkedList &other);
    LinkedList &operator=(const LinkedList &other);

    // move
    LinkedList(LinkedList &&old) noexcept;
    LinkedList &operator=(LinkedList &&old) noexcept;

    // Get

    int *get(size_t index);
    inline int *getFirst() { return get(0); }
    inline int *getLast() { return get(count() - 1); }

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

    void forEach(std::function<bool(NodeLink, size_t)> func);
    void forEach(std::function<bool(ConstNode, size_t)> func) const;

private:
    Node *head = nullptr;
    int count_ = 0;

    inline const bool indexInBounds(size_t i, bool includeLast = false) const
    {
        return i >= 0 && (includeLast ? i <= count_ : i < count_);
    }
};