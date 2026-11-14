#include <iostream>
#include <print>
#include <string>

#include "queue.hpp"

template <typename T>
void display(const std::string &name, const Queue<T> &queue)
{
    std::print("{}: ", name);
    bool first = true;
    queue.forEach([&first](const T &val) { //
        if (!first)
            std::print(" -> ");

        std::print("[ {} ]", val);
        first = false;
    });
    if (first) // for loop didn't run
        std::print("X");
    std::println(" -> X ");
}

template <typename T>
void enqueue(Queue<T> &queue, T value)
{
    queue.enqueue(value);
    std::println("+ {} ", value);
}

template <typename T>
std::optional<T> deque(Queue<T> &queue)
{
    std::optional<T> value = queue.deque();

    if (value.has_value())
        std::println("- {} ", value.value());
    else
        std::println("- Nothing");

    return value;
}

int main()
{
    Queue<int> intQueue;
    display("intQueue", intQueue);
    enqueue(intQueue, 10);
    enqueue(intQueue, 20);
    enqueue(intQueue, 30);
    display("intQueue", intQueue);

    deque(intQueue);
    display("intQueue", intQueue);
    enqueue(intQueue, 40);
    display("intQueue", intQueue);

    deque(intQueue);
    deque(intQueue);
    deque(intQueue);
    display("intQueue", intQueue);

    std::println();

    Queue<std::string> strQueue;
    strQueue.enqueue("Alice");
    strQueue.enqueue("Bob");
    strQueue.enqueue("Charlie");

    display("strQueue", strQueue);
    deque(strQueue);
    deque(strQueue);
    deque(strQueue);
    display("strQueue", strQueue);
    deque(strQueue);

    return 0;
}
