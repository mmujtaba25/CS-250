#include <chrono>
#include <cstdlib>
#include <ctime>
#include <print>
#include <string>

#include "linked_list.hpp"
#include "doubly_linked_list.hpp"

constexpr size_t N = 100'000;

void runSingly(LinkedList &list);
void runDoubly(DoublyLinkedList &list);

#define SHOW_TIME(...)                                                                      \
    do                                                                                      \
    {                                                                                       \
        std::chrono::time_point start = std::chrono::steady_clock::now();                   \
        __VA_ARGS__                                                                         \
        std::chrono::time_point end = std::chrono::steady_clock::now();                     \
        auto timeInUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start); \
        auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        std::println("Time For Program: {} ≈ {}", timeInUs, timeInMs);                      \
    } while (0);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::println("Usage: ./main --singly | --doubly");
        return 1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    LinkedList singly{};
    DoublyLinkedList doubly{};

    // same random integers in both lists
    for (size_t i = 0; i < N; i++)
    {
        const int value = std::rand() % 20;
        singly.insertStart(value);
        doubly.insertStart(value);
    }

    const std::string type = argv[1];

    if (type == "--singly")
        runSingly(singly);
    else if (type == "--doubly")
        runDoubly(doubly);
    else
    {
        std::println("Usage: ./main --singly | --doubly");
        return 1;
    }

    return 0;
}

void runSingly(LinkedList &list)
{
    Node *head = nullptr;
    Node *current = nullptr;

    // current pointer is already available before timing
    list.forEach([&](NodeLink node, size_t index) { //
        if (index == 0)
            head = node;

        if (index == N / 2)
        {
            current = node;
            return true;
        }

        return false;
    });

    SHOW_TIME(
        Node *next = current->next;
        std::println("Next: {}", next->data); //
    );

    SHOW_TIME(
        Node *previous = head;

        while (previous->next != current)
            previous = previous->next;

        std::println("Previous: {}", previous->data); //
    );

    const int insertAfterValue = std::rand() % 20;

    SHOW_TIME(
        Node *newNode = new Node{insertAfterValue, current->next};
        current->next = newNode;
        std::println("Inserted After: {}", insertAfterValue); //
    );

    const int insertBeforeValue = std::rand() % 20;

    SHOW_TIME(
        Node *previous = head;

        while (previous->next != current)
            previous = previous->next;

        previous->next = new Node{insertBeforeValue, current};
        std::println("Inserted Before: {}", insertBeforeValue); //
    );

    SHOW_TIME(
        Node *previous = head;

        while (previous->next != current)
            previous = previous->next;

        previous->next = current->next;
        delete current;
        current = nullptr;

        std::println("Deleted"); //
    );
}

void runDoubly(DoublyLinkedList &list)
{
    DoubleSidedNode *current = nullptr;

    // current pointer is already available before timing
    list.forEach([&](DoubleSidedNodeLink node, size_t index) { //
        if (index == N / 2)
        {
            current = node;
            return true;
        }

        return false;
    });

    SHOW_TIME(
        DoubleSidedNode *next = current->next;
        std::println("Next: {}", next->data); //
    );

    SHOW_TIME(
        DoubleSidedNode *previous = current->previous;
        std::println("Previous: {}", previous->data); //
    );

    const int insertAfterValue = std::rand() % 20;

    SHOW_TIME(
        DoubleSidedNode *newNode = new DoubleSidedNode{
            insertAfterValue,
            current,
            current->next};

        if (current->next != nullptr) current->next->previous = newNode;

        current->next = newNode;

        std::println("Inserted After: {}", insertAfterValue); //
    );

    const int insertBeforeValue = std::rand() % 20;

    SHOW_TIME(
        DoubleSidedNode *newNode = new DoubleSidedNode{
            insertBeforeValue,
            current->previous,
            current};

        current->previous->next = newNode; current->previous = newNode;

        std::println("Inserted Before: {}", insertBeforeValue); //
    );

    SHOW_TIME(
        current->previous->next = current->next;

        if (current->next != nullptr)
            current->next->previous = current->previous;

        delete current;
        current = nullptr;

        std::println("Deleted"); //
    );
}
