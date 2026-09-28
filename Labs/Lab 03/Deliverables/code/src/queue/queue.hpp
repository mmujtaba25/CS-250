#pragma once

#include <functional>

#include "linked_list.hpp"

// private inherited LinkedList so Queue callers cannot call LinkedList functions like insertAt etc.
class Queue : private LinkedList
{
public:
    bool enqueue(int value);
    int dequeue();
    int peek();

    void forEach(std::function<void(ConstNode, size_t)> func) const { LinkedList::forEach(func); }
};
