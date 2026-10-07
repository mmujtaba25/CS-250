#include <chrono>
#include <cstdlib>
#include <ctime>
#include <print>

#include "linked_list.hpp"

constexpr size_t N = 100'000;

#define SHOW_TIME(FUNC)                                                                     \
    do                                                                                      \
    {                                                                                       \
        std::chrono::time_point start = std::chrono::steady_clock::now();                   \
        FUNC                                                                                \
            std::chrono::time_point end = std::chrono::steady_clock::now();                 \
        auto timeInUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
        auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        std::println("Time For Program: {} ≈ {}", timeInUs, timeInMs);                      \
    } while (0);

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    LinkedList headOnlyList{};
    LinkedList pointerList{};

    for (size_t i = 0; i < N; i++)
    {
        const int value = std::rand() % 20;
        headOnlyList.insertStart(value);
        pointerList.insertStart(value);
    }

    SHOW_TIME(
        std::println("Delete last node using only head:");
        headOnlyList.removeLast(); //
    );

    // get pointers
    Node *previous = nullptr;
    Node *target = nullptr;
    pointerList.forEach([&](NodeLink current, size_t index) { //
        if (index == N - 2)
            previous = current;

        if (index == N - 1)
        {
            target = current;
            return true;
        }

        return false;
    });

    SHOW_TIME(
        std::println("Delete node with previous and target pointers:");
        previous->next = target->next;
        delete target; //
    );

    return 0;
}
