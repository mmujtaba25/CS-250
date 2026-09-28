#include "linked_list.hpp"

// MARK: Copy

LinkedList::LinkedList(const LinkedList &other) : head(nullptr)
{
    // copy each node;
    // no need to cleanup, new object being created with head initialized to null
    other.forEach([this](ConstNode node, size_t) { //
        this->insertEnd(node->data);
    });
}

LinkedList &LinkedList::operator=(const LinkedList &other)
{
    // if same
    if (this == &other)
        return *this;

    // clear self
    this->removeAll();

    // insert elements into other
    other.forEach([this](ConstNode node, size_t) { //
        this->insertEnd(node->data);
    });

    return *this;
}

// MARK: Move

LinkedList::LinkedList(LinkedList &&old) noexcept : head(old.head) // repoint head
{
    old.head = nullptr; // remove reference from old one
}

LinkedList &LinkedList::operator=(LinkedList &&old) noexcept
{
    // if same
    if (this == &old)
        return *this;

    // remove previous elements
    this->removeAll();

    // repoint head
    head = old.head;
    old.head = nullptr;

    return *this;
}

// MARK: Get

/// `nullptr` = not found
int *LinkedList::get(size_t index)
{
    if (index < 0 || !head)
        return nullptr;

    int *result = nullptr;

    forEach([&result, &index](NodeLink current, size_t currentIndex) { //
        if (currentIndex == index)
            result = &(current->data);
    });

    // either nullptr or actual value
    return result;
}

// MARK: Insert

/// the value is inserted at index `index`, get using `get(index)`;
bool LinkedList::insertAt(int value, size_t index)
{
    if (!indexInBounds(index, true))
        return false;

    // inserting into empty list
    // for each cannot run in a empty list
    if (head == nullptr)
    {
        head = new Node{value, nullptr};
        return true;
    }

    bool inserted = false;

    // inserting at the end
    if (index == count())
    {
        forEach([&value, &inserted](NodeLink current, size_t index) { //
            if (current->next == nullptr && !inserted)
            {
                current->next = new Node{value, nullptr};

                // make sure executes once
                inserted = true;
            }
        });

        return inserted;
    }

    // inserting in the middle
    forEach([&value, &index, &inserted](NodeLink current, size_t currentIndex) { //
        if (currentIndex == index && !inserted)
        {
            // create node and point to current node
            Node *newNode = new Node{value, current};
            // point current to new node
            current = newNode;

            inserted = true;
        }
    });

    return true;
}

// MARK: Remove

bool LinkedList::removeIf(const std::function<bool(const int &, size_t)> &predicate)
{
    // nothing to remove
    if (head == nullptr)
        return false;

    // chekc if predicate applies to head
    if (predicate(head->data, 0))
    {
        Node *old = head;
        // move head by one
        head = head->next;
        // free up old space
        delete old;

        return true;
    }

    bool removed = false;
    // check predicate : all except head
    forEach([&predicate, &removed](NodeLink current, size_t index) { //
        // run only once && not empty or last
        if (removed || current == nullptr || current->next == nullptr)
            return; // equivalent to continue in a for loop

        // check if predicate applies to the next node
        Node *nextNode = current->next;
        if (predicate(nextNode->data, index + 1))
        {
            // point current's next to the next, next node
            current->next = current->next->next;
            // delete nextNode
            delete nextNode;

            removed = true;
        }
    });

    return removed;
}

bool LinkedList::removeAt(int index)
{
    if (!indexInBounds(index))
        return false;

    return removeIf([&index](const int &, size_t i) { //
        return i == index;
    });
}

void LinkedList::removeAll()
{
    // keep deleting until head is null as well
    // i.e memory at head is free as well
    while (head != nullptr)
    {
        Node *next = head->next;
        delete head;
        head = next;
    }
}

// MARK: Count

size_t LinkedList::count() const
{
    size_t count = 0;

    forEach([&count](ConstNode data, size_t index) { //
        count++;
    });

    return count;
}

// MARK: For Each

/// @brief Runs between index 0 to `count()` - 1
///
/// MUTABLE
/// @param func `(NodeLink current, size_t currentIndex) -> void`
void LinkedList::forEach(std::function<void(NodeLink, size_t)> func)
{
    size_t index = 0;
    Node **current = &head;
    while (*current != nullptr)
    {
        // pass reference to the node itself
        func(*current, index);

        // move to next value
        current = &((*current)->next);
        index++;
    }
}

/// @brief Runs between index 0 to `count()` - 1
///
/// IMMUTABLE
/// @param func `(ConstNode current, size_t currentIndex) -> void`
void LinkedList::forEach(std::function<void(ConstNode, size_t)> func) const
{
    size_t index = 0;
    Node *current = head;
    while (current != nullptr)
    {
        // pass const "copy" of the node
        func(current, index);

        // move to next value
        current = current->next;
        index++;
    }
}
