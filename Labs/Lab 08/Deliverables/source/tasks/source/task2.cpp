#include <iostream>
#include <print>
#include <string>

#include "circular_queue.hpp"

template <typename T>
void display(const std::string &name, const CircularQueue<T> &queue)
{
    std::print("{}: ", name);
    bool first = true;

    queue.forEach([&first](const T &val) { //
        if (!first)
            std::print(" -> ");

        std::print("( {} )", val);
        first = false;
    });

    if (first)
        std::println("Empty");
    else
        std::println(" -> (FIRST)");
}

template <typename T>
void enqueue(CircularQueue<T> &queue, T value)
{
    if (queue.enqueue(value))
        std::println("+ {}", value);
    else
        std::println("  {} (FULL)", value);
}

template <typename T>
std::optional<T> deque(CircularQueue<T> &queue)
{
    std::optional<T> value = queue.deque();

    if (value.has_value())
        std::println("- {}", value.value());
    else
        std::println("- Nothing");

    return value;
}

int main()
{
    CircularQueue<std::string> circular(3);

    display("circular", circular);
    enqueue(circular, std::string("Alice"));
    enqueue(circular, std::string("Bob"));
    enqueue(circular, std::string("Charlie"));
    display("circular", circular);
    std::println();

    enqueue(circular, std::string("David"));
    display("circular", circular);
    std::println();

    deque(circular);
    deque(circular);
    enqueue(circular, std::string("David"));
    deque(circular);
    display("circular", circular);

    return 0;
}
