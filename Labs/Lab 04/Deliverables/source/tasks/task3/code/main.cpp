#include <chrono>
#include <cstdlib>
#include <ctime>
#include <print>
#include <string>

#include "linked_list.hpp"

constexpr size_t N = 100'000;

void accessFirst(int *array, LinkedList &list);
void accessLast(int *array, LinkedList &list);
void accessRandom(int *array, LinkedList &list);
void findValue(int *array, LinkedList &list, int value);

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
    if (argc < 2)
    {
        std::println("Usage: ./task-3 --first | --last | --random | --find <value>");
        return 1;
    }

    // seed rand
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int *array = new int[N];
    LinkedList list{};

    for (size_t i = N; i > 0; i--)
    {
        const int value = std::rand() % 20;
        array[i - 1] = value;
        // list inserts in reverse order
        // 4, 3, 2, 1
        list.insertStart(value);
    }

    std::string operation = argv[1];

    if (operation == "--first")
        accessFirst(array, list);
    else if (operation == "--last")
        accessLast(array, list);
    else if (operation == "--random")
        accessRandom(array, list);
    else if (operation == "--find" && argc >= 3)
        findValue(array, list, std::atoi(argv[2]));
    else
        std::println("Invalid arguments");

    delete[] array;
    return 0;
}

void accessFirst(int *array, LinkedList &list)
{
    SHOW_TIME(
        std::println("Dynamic Array:");
        int value = array[0];
        std::println("Value: {}", value); //
    );

    SHOW_TIME(
        size_t examined = 0;
        int value = *list.get(0, &examined);
        std::println("Singly Linked List:");
        std::println("Value: {}; visited {} nodes", value, examined); //
    );
}

void accessLast(int *array, LinkedList &list)
{
    SHOW_TIME(
        std::println("Dynamic Array:");
        int value = array[N - 1];
        std::println("Value: {}", value); //
    );

    SHOW_TIME(
        size_t examined = 0;
        int value = *list.get(N - 1, &examined);
        std::println("Singly Linked List:");
        std::println("Value: {}; visited {} nodes", value, examined); //
    );
}

void accessRandom(int *array, LinkedList &list)
{
    size_t index = std::rand() % N;

    SHOW_TIME(
        std::println("Dynamic Array:");
        int value = array[index];
        std::println("Value: {}", value); //
    );

    SHOW_TIME(
        size_t examined = 0;
        int value = *list.get(index, &examined);
        std::println("Singly Linked List:");
        std::println("Value: {}; visited {} nodes", value, examined); //
    );
}

void findValue(int *array, LinkedList &list, int value)
{

    SHOW_TIME(
        int arrayIndex = -1;
        std::println("Dynamic Array:");
        for (size_t i = 0; i < N; i++) {
            if (array[i] == value)
            {
                arrayIndex = static_cast<int>(i);
                break;
            }
        } //
        std::println("Found {} at index {}", value, arrayIndex); //
    );

    SHOW_TIME(
        int listIndex = -1;
        std::println("Singly Linked List:");
        list.forEach([&](ConstNode node, size_t index) { //
            if (node->data == value)
            {
                listIndex = static_cast<int>(index);
                return true;
            }
            return false;
        });
        std::println("Found {} at index {}", value, listIndex); //
    );
}
