#include "queue.hpp"

bool Queue::enqueue(int value)
{
    return this->insertEnd(value);
}

int Queue::dequeue()
{
    int value = peek();
    this->removeFirst();
    return value;
}

int Queue::peek()
{
    return *(this->getFirst());
}