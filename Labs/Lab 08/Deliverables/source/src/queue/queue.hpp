#pragma once

#include <optional>
#include <functional>

class Queue
{
private: // MARK: Internal
    using T = int;
    using ConstT = const T &;

    struct InternalNode
    {
        T data;
        InternalNode *next = nullptr;
        InternalNode *previous = nullptr;
    };

private: // MARK: Queue
    InternalNode *head = nullptr;
    InternalNode *tail = nullptr;

public:
    Queue() = default;
    ~Queue()
    {
        while (head != nullptr)
        {
            InternalNode *next = head->next;
            delete head;
            head = next;
        }
    }

    // copy constructors: not allowed
    Queue(const Queue &) = delete;
    Queue &operator=(const Queue &) = delete;

    void enqueue(T value)
    {
        InternalNode *node = new InternalNode{
            .data = value,
            .next = nullptr,
            .previous = tail //
        };

        if (tail)
            tail->next = node;
        else // if list empty, head at new node
            head = node;

        // repoint
        tail = node;
    }

    std::optional<T> deque()
    {
        if (head == nullptr)
            return std::nullopt;

        InternalNode *oldHead = head;
        T value = oldHead->data;

        head = head->next;
        if (head != nullptr) // next element exists
            head->previous = nullptr;
        else // empty list
            tail = nullptr;

        delete oldHead;
        return value;
    }

    // for displaying the entire queue
    void forEach(std::function<void(ConstT)> callable) const
    {
        InternalNode *cursor = head;
        while (cursor != nullptr)
        {
            callable(cursor->data);
            cursor = cursor->next;
        }
    }
};
