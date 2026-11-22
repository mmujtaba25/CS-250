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

    template <UnsignedNumeric U>
    friend class LinkedListHashTable;

public:
    bool exists() const { return !list.empty(); }
    const typename LinkedList<T>::Node *getHead() const { return list.getHead(); }
};

template <UnsignedNumeric T>
class LinkedListHashTable
{
private:
    SlotNode<T> *buckets;
    size_t tableSize;
    size_t elementCount = 0;

    size_t hash(const T &element) const { return element % tableSize; }

public:
    explicit LinkedListHashTable(size_t initialSize = 24) : tableSize(initialSize > 0 ? initialSize : 24)
    {
        buckets = new SlotNode<T>[tableSize];
    }

    ~LinkedListHashTable() { delete[] buckets; }

    LinkedListHashTable(const LinkedListHashTable &) = delete;
    LinkedListHashTable &operator=(const LinkedListHashTable &) = delete;

    void insert(const T &newValue)
    {
        buckets[hash(newValue)].insert(newValue);
        elementCount++;
    }

    bool remove(const T &element)
    {
        if (!buckets[hash(element)].remove(element))
            return false;

        elementCount--;
        return true;
    }

    std::optional<size_t> search(const T &element) const
    {
        size_t hashed_index = hash(element);
        if (buckets[hashed_index].contains(element))
            return hashed_index;

        return std::nullopt;
    }

    void resize(size_t newSize)
    {
        if (newSize == 0 || newSize == tableSize)
            return;

        SlotNode<T> *newBuckets = new SlotNode<T>[newSize];

        for (size_t i = 0; i < tableSize; i++)
        {
            for (const auto *curr = buckets[i].getHead(); curr != nullptr; curr = curr->next)
                newBuckets[curr->value % newSize].insert(curr->value);
        }

        delete[] buckets; // Automatically frees old chain nodes via ~LinkedList()
        buckets = newBuckets;
        tableSize = newSize;
    }

    size_t size() const { return elementCount; }
    size_t capacity() const { return tableSize; }
    double loadFactor() const { return static_cast<double>(elementCount) / tableSize; }

    void forEach(std::function<void(size_t, const SlotNode<T> &)> callable) const
    {
        for (size_t i = 0; i < tableSize; i++)
            callable(i, buckets[i]);
    }
};
