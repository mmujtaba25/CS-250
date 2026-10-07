#include <chrono>
#include <print>
#include <random>
#include <optional>

#include "linked_list.hpp"

struct ParsedArgs
{
    int n = 10;
};

bool parseArgs(int argc, char *argv[], ParsedArgs *returnValue)
{
    if (argc < 2)
        return false;

    try
    {
        returnValue->n = std::stoi(argv[1]);
        return true;
    }
    catch (const std::exception &)
    {
        return false;
    }
}

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

int main(int argc, char *argv[])
{
    ParsedArgs args;
    parseArgs(argc, argv, &args);

    LinkedList myLinkedList{};
    for (size_t i = 0; i < args.n; i++)
    {
        myLinkedList.insertAt(std::rand() % 20, 0);
    }

    SHOW_TIME(
        int index = 0;
        size_t nodes_examined = 0;
        std::println("@index{{{}}}: {}; visited {} nodes", index, *myLinkedList.get(index, &nodes_examined), nodes_examined); //
    );

    SHOW_TIME(
        int index = myLinkedList.count() / 2;
        size_t nodes_examined = 0;
        std::println("@index{{{}}}: {}; visited {} nodes", index, *myLinkedList.get(index, &nodes_examined), nodes_examined); //
    );

    SHOW_TIME(
        int index = myLinkedList.count() - 1;
        size_t nodes_examined = 0;
        std::println("@index{{{}}}: {}; visited {} nodes", index, *myLinkedList.get(index, &nodes_examined), nodes_examined); //
    );

    return 0;
}
