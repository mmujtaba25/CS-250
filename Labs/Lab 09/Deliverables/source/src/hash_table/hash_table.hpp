#pragma once

#include <functional>
#include <optional>
#include <concepts>

template <typename T>
class LinkedList
{
public:
    struct Node
    {
        T value;
        Node *next = nullptr;
    };

private:
    Node *head = nullptr;

public:
    LinkedList() = default;
    LinkedList(const LinkedList &) = delete;
    LinkedList &operator=(const LinkedList &) = delete;

    ~LinkedList()
    {
        while (head != nullptr)
        {
            Node *toDelete = head;
            head = head->next;
            delete toDelete;
        }
    }

    void insert(const T &val) { head = new Node{val, head}; }

    bool remove(const T &val)
    {
        Node *prev = nullptr, *curr = head;
        while (curr != nullptr)
        {
            if (curr->value == val)
            {
                (prev ? prev->next : head) = curr->next;
                delete curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    bool contains(const T &val) const
    {
        for (const Node *curr = head; curr != nullptr; curr = curr->next)
            if (curr->value == val)
                return true;
        return false;
    }

    bool empty() const { return head == nullptr; }
    const Node *getHead() const { return head; }
};

template <typename T>
concept UnsignedNumeric = // allow only unsigned since -3 % x is < 0 (out of bounds)
    std::unsigned_integral<T> && !std::same_as<T, bool> &&
    !std::same_as<T, char8_t> && !std::same_as<T, char16_t> &&
    !std::same_as<T, char32_t> && !std::same_as<T, wchar_t>;

template <UnsignedNumeric T>
class SlotNode
{
private:
    LinkedList<T> list;

    void insert(const T &val) { list.insert(val); }
    bool remove(const T &val) { return list.remove(val); }
    bool contains(const T &val) const { return list.contains(val); }

    template <UnsignedNumeric U, size_t S>
        requires(S > 0)
    friend class LinkedListHashTable;

public:
    bool exists() const { return !list.empty(); }
    const typename LinkedList<T>::Node *getHead() const { return list.getHead(); }
};

template <UnsignedNumeric T, size_t S = 7>
    requires(S > 0)
class LinkedListHashTable
{
private:
    SlotNode<T> *buckets;

    size_t hash(const T &element) const { return element % S; }

public:
    LinkedListHashTable() { buckets = new SlotNode<T>[S]; }
    ~LinkedListHashTable() { delete[] buckets; }

    // no copying
    LinkedListHashTable(const LinkedListHashTable &other) = delete;
    LinkedListHashTable &operator=(const LinkedListHashTable &other) = delete;

    void insert(const T &newValue) { buckets[hash(newValue)].insert(newValue); }
    bool remove(const T &element) { return buckets[hash(element)].remove(element); }

    std::optional<size_t> search(const T &element) const
    {
        size_t hashed_index = hash(element);
        if (buckets[hashed_index].contains(element))
            return hashed_index;

        return std::nullopt;
    }

    // Immutable forEach passing bucket index and head SlotNode reference
    void forEach(std::function<void(size_t, const SlotNode<T> &)> callable) const
    {
        for (size_t i = 0; i < S; i++)
            callable(i, buckets[i]);
    }
};
