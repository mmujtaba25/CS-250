#include <chrono>
#include <print>

struct Node
{
    int data;
    Node *next = nullptr;
};

void addNodeLast(Node *head, int data)
{
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp->next == nullptr)
        {
            Node *newNode = new Node{.data = data};
            temp->next = newNode;
            break;
        }
        temp = temp->next;
    }
}

int main()
{
    Node *head = new Node{.data = 0};
    for (size_t i = 1; i < 10; i++)
    {
        addNodeLast(head, i);
    }

    std::chrono::time_point start = std::chrono::steady_clock::now();

    Node *temp = head;
    while (temp != nullptr)
    {
        std::print("{}, ", temp->data);
        temp = temp->next;
    }
    std::println();

    std::chrono::time_point end = std::chrono::steady_clock::now();

    auto timeInUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::println("Time For Program: {} ≈ {}", timeInUs, timeInMs);

    return 0;
}
