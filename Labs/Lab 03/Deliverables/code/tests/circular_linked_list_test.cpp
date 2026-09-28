#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "circular_linked_list.hpp"

// ---------------------------------------------------------
// Helpers
// ---------------------------------------------------------

// all values, in order, read through the const forEach
static std::vector<int> values(const CircularLinkedList &list)
{
    std::vector<int> result;

    list.forEach([&result](ConstNode node, size_t) { //
        result.push_back(node->data);
    });

    return result;
}

// the last node must point back to the first one,
// and following `next` from the first node must return to it after exactly count() steps
static bool isCircular(const CircularLinkedList &list)
{
    ConstNode first = nullptr;
    ConstNode last = nullptr;
    size_t visited = 0;

    list.forEach([&](ConstNode node, size_t index) { //
        if (index == 0)
            first = node;
        last = node;
        visited++;
    });

    // empty list: nothing to link
    if (first == nullptr)
        return visited == 0;

    if (last->next != first)
        return false;

    ConstNode current = first;
    for (size_t i = 0; i < visited; ++i)
    {
        if (current == nullptr)
            return false;
        current = current->next;
    }

    return current == first;
}

// ---------------------------------------------------------
// Construction / count
// ---------------------------------------------------------

TEST(CircularLinkedListTest, DefaultConstructorCreatesEmptyList)
{
    CircularLinkedList list;

    EXPECT_EQ(list.count(), 0);
    EXPECT_EQ(list.getFirst(), nullptr);
    EXPECT_EQ(list.getLast(), nullptr);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, ValueConstructorCreatesSingleElement)
{
    CircularLinkedList list(42);

    ASSERT_EQ(list.count(), 1);

    ASSERT_NE(list.get(0), nullptr);
    EXPECT_EQ(*list.get(0), 42);
}

TEST(CircularLinkedListTest, ValueConstructorNodePointsToItself)
{
    CircularLinkedList list(42);

    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, CountTracksNumberOfElements)
{
    CircularLinkedList list;

    EXPECT_EQ(list.count(), 0);

    EXPECT_TRUE(list.insertEnd(10));
    EXPECT_EQ(list.count(), 1);

    EXPECT_TRUE(list.insertEnd(20));
    EXPECT_EQ(list.count(), 2);

    EXPECT_TRUE(list.insertEnd(30));
    EXPECT_EQ(list.count(), 3);
}

// ---------------------------------------------------------
// get
// ---------------------------------------------------------

TEST(CircularLinkedListTest, GetReturnsCorrectElements)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_NE(list.get(0), nullptr);
    ASSERT_NE(list.get(1), nullptr);
    ASSERT_NE(list.get(2), nullptr);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_EQ(*list.get(2), 30);
}

TEST(CircularLinkedListTest, GetReturnsNullForOutOfBoundsIndex)
{
    CircularLinkedList list;

    EXPECT_EQ(list.get(0), nullptr);

    ASSERT_TRUE(list.insertEnd(10));

    EXPECT_EQ(list.get(1), nullptr);
    EXPECT_EQ(list.get(100), nullptr);
}

TEST(CircularLinkedListTest, GetDoesNotWrapAround)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    // index == count() must not come back around to the head
    EXPECT_EQ(list.get(2), nullptr);
    EXPECT_EQ(list.get(3), nullptr);
}

TEST(CircularLinkedListTest, GetFirstReturnsFirstElement)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_NE(list.getFirst(), nullptr);
    EXPECT_EQ(*list.getFirst(), 10);
}

TEST(CircularLinkedListTest, GetLastReturnsLastElement)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_NE(list.getLast(), nullptr);
    EXPECT_EQ(*list.getLast(), 30);
}

TEST(CircularLinkedListTest, GetFirstAndLastAreSameForSingleElement)
{
    CircularLinkedList list(7);

    ASSERT_NE(list.getFirst(), nullptr);
    EXPECT_EQ(list.getFirst(), list.getLast());
}

// ---------------------------------------------------------
// insertStart
// ---------------------------------------------------------

TEST(CircularLinkedListTest, InsertStartWorksOnEmptyList)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertStart(10));

    ASSERT_EQ(list.count(), 1);
    ASSERT_NE(list.get(0), nullptr);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertStartMovesExistingElementsRight)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertStart(20));
    ASSERT_TRUE(list.insertStart(10));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
}

TEST(CircularLinkedListTest, InsertStartOnSingleElementKeepsCircle)
{
    CircularLinkedList list(20);

    ASSERT_TRUE(list.insertStart(10));

    EXPECT_EQ(values(list), (std::vector<int>{10, 20}));
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RepeatedInsertStartKeepsOrderAndCircle)
{
    CircularLinkedList list;

    for (int i = 5; i >= 1; --i)
    {
        ASSERT_TRUE(list.insertStart(i));
        ASSERT_TRUE(isCircular(list));
    }

    EXPECT_EQ(values(list), (std::vector<int>{1, 2, 3, 4, 5}));
}

// ---------------------------------------------------------
// insertEnd
// ---------------------------------------------------------

TEST(CircularLinkedListTest, InsertEndWorksOnEmptyList)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.get(0), 10);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertEndAppendsValues)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_EQ(*list.get(2), 30);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertEndDoesNotMoveHead)
{
    CircularLinkedList list(10);

    int *first = list.getFirst();

    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    EXPECT_EQ(list.getFirst(), first);
}

// ---------------------------------------------------------
// insertAt
// ---------------------------------------------------------

TEST(CircularLinkedListTest, InsertAtBeginning)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.insertAt(10, 0));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_EQ(*list.get(2), 30);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertAtMiddle)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.insertAt(20, 1));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_EQ(*list.get(2), 30);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertAtBeforeLast)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(40));

    ASSERT_TRUE(list.insertAt(30, 2));

    EXPECT_EQ(values(list), (std::vector<int>{10, 20, 30, 40}));
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertAtEnd)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_TRUE(list.insertAt(30, 2));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(2), 30);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertAtZeroWorksOnEmptyList)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertAt(10, 0));

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.get(0), 10);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, InsertAtRejectsInvalidIndex)
{
    CircularLinkedList list;

    EXPECT_FALSE(list.insertAt(10, 1));

    ASSERT_TRUE(list.insertAt(10, 0));

    EXPECT_FALSE(list.insertAt(30, 2));

    EXPECT_EQ(values(list), (std::vector<int>{10}));
}

// ---------------------------------------------------------
// removeAt
// ---------------------------------------------------------

TEST(CircularLinkedListTest, RemoveAtBeginning)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeAt(0));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 20);
    EXPECT_EQ(*list.get(1), 30);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveAtMiddle)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeAt(1));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 30);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveAtEnd)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeAt(2));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveAtOnlyElement)
{
    CircularLinkedList list(10);

    ASSERT_TRUE(list.removeAt(0));

    EXPECT_EQ(list.count(), 0);
    EXPECT_EQ(list.getFirst(), nullptr);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveAtRejectsInvalidIndex)
{
    CircularLinkedList list;

    EXPECT_FALSE(list.removeAt(0));

    ASSERT_TRUE(list.insertEnd(10));

    EXPECT_FALSE(list.removeAt(1));
    EXPECT_FALSE(list.removeAt(100));

    EXPECT_EQ(values(list), (std::vector<int>{10}));
}

TEST(CircularLinkedListTest, RemoveAtEveryIndexLeavesValidCircle)
{
    for (int removeIndex = 0; removeIndex < 5; ++removeIndex)
    {
        CircularLinkedList list;
        for (int i = 0; i < 5; ++i)
            ASSERT_TRUE(list.insertEnd(i));

        ASSERT_TRUE(list.removeAt(removeIndex));

        std::vector<int> expected;
        for (int i = 0; i < 5; ++i)
            if (i != removeIndex)
                expected.push_back(i);

        EXPECT_EQ(values(list), expected);
        EXPECT_TRUE(isCircular(list));
    }
}

// ---------------------------------------------------------
// removeFirst / removeLast
// ---------------------------------------------------------

TEST(CircularLinkedListTest, RemoveFirstRemovesHead)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_TRUE(list.removeFirst());

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.getFirst(), 20);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveLastRemovesTail)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeLast());

    ASSERT_EQ(list.count(), 2);
    EXPECT_EQ(*list.getLast(), 20);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveFirstOnEmptyListReturnsFalse)
{
    CircularLinkedList list;

    EXPECT_FALSE(list.removeFirst());
}

TEST(CircularLinkedListTest, RemoveLastOnEmptyListReturnsFalse)
{
    CircularLinkedList list;

    EXPECT_FALSE(list.removeLast());
}

TEST(CircularLinkedListTest, RemoveFirstUntilEmpty)
{
    CircularLinkedList list;
    for (int i = 0; i < 4; ++i)
        ASSERT_TRUE(list.insertEnd(i));

    for (int i = 0; i < 4; ++i)
    {
        ASSERT_NE(list.getFirst(), nullptr);
        EXPECT_EQ(*list.getFirst(), i);
        ASSERT_TRUE(list.removeFirst());
        ASSERT_TRUE(isCircular(list));
    }

    EXPECT_EQ(list.count(), 0);
    EXPECT_FALSE(list.removeFirst());
}

TEST(CircularLinkedListTest, RemoveLastUntilEmpty)
{
    CircularLinkedList list;
    for (int i = 0; i < 4; ++i)
        ASSERT_TRUE(list.insertEnd(i));

    for (int i = 3; i >= 0; --i)
    {
        ASSERT_NE(list.getLast(), nullptr);
        EXPECT_EQ(*list.getLast(), i);
        ASSERT_TRUE(list.removeLast());
        ASSERT_TRUE(isCircular(list));
    }

    EXPECT_EQ(list.count(), 0);
    EXPECT_FALSE(list.removeLast());
}

TEST(CircularLinkedListTest, ListIsReusableAfterBecomingEmpty)
{
    CircularLinkedList list(1);

    ASSERT_TRUE(list.removeFirst());
    ASSERT_EQ(list.count(), 0);

    ASSERT_TRUE(list.insertEnd(2));
    ASSERT_TRUE(list.insertStart(1));

    EXPECT_EQ(values(list), (std::vector<int>{1, 2}));
    EXPECT_TRUE(isCircular(list));
}

// ---------------------------------------------------------
// removeIf
// ---------------------------------------------------------

TEST(CircularLinkedListTest, RemoveIfRemovesMatchingElement)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    bool removed = list.removeIf(
        [](const int &value, size_t)
        {
            return value == 20;
        });

    EXPECT_TRUE(removed);

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 30);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveIfCanRemoveHead)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_TRUE(list.removeIf(
        [](const int &value, size_t)
        {
            return value == 10;
        }));

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.get(0), 20);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveIfCanRemoveLast)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeIf(
        [](const int &value, size_t)
        {
            return value == 30;
        }));

    ASSERT_EQ(list.count(), 2);
    EXPECT_EQ(*list.getLast(), 20);
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveIfUsesCorrectIndex)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeIf(
        [](const int &, size_t index)
        {
            return index == 1;
        }));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 30);
}

TEST(CircularLinkedListTest, RemoveIfReturnsFalseWhenNothingMatches)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    bool removed = list.removeIf(
        [](const int &value, size_t)
        {
            return value == 999;
        });

    EXPECT_FALSE(removed);

    EXPECT_EQ(list.count(), 2);
}

TEST(CircularLinkedListTest, RemoveIfOnEmptyListReturnsFalse)
{
    CircularLinkedList list;

    EXPECT_FALSE(list.removeIf([](const int &, size_t) { return true; }));
}

TEST(CircularLinkedListTest, RemoveIfRemovesOnlyFirstMatch)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeIf([](const int &value, size_t) { return value == 20; }));

    EXPECT_EQ(values(list), (std::vector<int>{10, 20, 30}));
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, RemoveIfDoesNotWrapAroundToHead)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    // the last node must not check head again as index 2
    EXPECT_FALSE(list.removeIf([](const int &, size_t index) { return index >= 2; }));

    EXPECT_EQ(values(list), (std::vector<int>{10, 20}));
}

TEST(CircularLinkedListTest, RemoveIfSeesEachIndexOnce)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    std::vector<size_t> seen;
    EXPECT_FALSE(list.removeIf(
        [&seen](const int &, size_t index)
        {
            seen.push_back(index);
            return false;
        }));

    EXPECT_EQ(seen, (std::vector<size_t>{0, 1, 2}));
}

TEST(CircularLinkedListTest, RemoveIfOnSingleElementEmptiesList)
{
    CircularLinkedList list(10);

    ASSERT_TRUE(list.removeIf([](const int &value, size_t) { return value == 10; }));

    EXPECT_EQ(list.count(), 0);
    EXPECT_EQ(list.getFirst(), nullptr);
}

// ---------------------------------------------------------
// removeAll
// ---------------------------------------------------------

TEST(CircularLinkedListTest, RemoveAllEmptiesList)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    list.removeAll();

    EXPECT_EQ(list.count(), 0);
    EXPECT_EQ(list.getFirst(), nullptr);
}

TEST(CircularLinkedListTest, RemoveAllOnEmptyAndSingleElement)
{
    CircularLinkedList empty;
    empty.removeAll();
    EXPECT_EQ(empty.count(), 0);

    CircularLinkedList single(1);
    single.removeAll();
    EXPECT_EQ(single.count(), 0);

    // twice in a row is fine
    single.removeAll();
    EXPECT_EQ(single.count(), 0);
}

// ---------------------------------------------------------
// forEach
// ---------------------------------------------------------

TEST(CircularLinkedListTest, ConstForEachVisitsEveryElementInOrder)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    const CircularLinkedList &constList = list;

    std::vector<int> values;

    constList.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    ASSERT_EQ(values.size(), 3);

    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

TEST(CircularLinkedListTest, ForEachProvidesCorrectIndexes)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    const CircularLinkedList &constList = list;

    std::vector<size_t> indexes;

    constList.forEach(
        [&indexes](ConstNode, size_t index)
        {
            indexes.push_back(index);
        });

    ASSERT_EQ(indexes.size(), 3);

    EXPECT_EQ(indexes[0], 0);
    EXPECT_EQ(indexes[1], 1);
    EXPECT_EQ(indexes[2], 2);
}

TEST(CircularLinkedListTest, ForEachVisitsEachNodeOnceSingleElement)
{
    CircularLinkedList list(10);

    size_t calls = 0;
    list.forEach([&calls](NodeLink, size_t) { calls++; });

    EXPECT_EQ(calls, 1);
}

TEST(CircularLinkedListTest, ForEachOnEmptyListNeverCalls)
{
    CircularLinkedList list;
    const CircularLinkedList &constList = list;

    size_t calls = 0;
    list.forEach([&calls](NodeLink, size_t) { calls++; });
    constList.forEach([&calls](ConstNode, size_t) { calls++; });

    EXPECT_EQ(calls, 0);
}

TEST(CircularLinkedListTest, MutableForEachCanChangeValues)
{
    CircularLinkedList list;

    ASSERT_TRUE(list.insertEnd(1));
    ASSERT_TRUE(list.insertEnd(2));
    ASSERT_TRUE(list.insertEnd(3));

    list.forEach([](NodeLink node, size_t index) { //
        node->data = node->data * 10 + static_cast<int>(index);
    });

    EXPECT_EQ(values(list), (std::vector<int>{10, 21, 32}));
    EXPECT_TRUE(isCircular(list));
}

// ---------------------------------------------------------
// Copy / move
// ---------------------------------------------------------

TEST(CircularLinkedListTest, CopyConstructorCopiesValues)
{
    CircularLinkedList original;
    ASSERT_TRUE(original.insertEnd(10));
    ASSERT_TRUE(original.insertEnd(20));
    ASSERT_TRUE(original.insertEnd(30));

    CircularLinkedList copy(original);

    EXPECT_EQ(values(copy), (std::vector<int>{10, 20, 30}));
    EXPECT_TRUE(isCircular(copy));
}

TEST(CircularLinkedListTest, CopyIsIndependentOfOriginal)
{
    CircularLinkedList original;
    ASSERT_TRUE(original.insertEnd(10));
    ASSERT_TRUE(original.insertEnd(20));

    CircularLinkedList copy(original);

    *copy.get(0) = 99;
    ASSERT_TRUE(copy.insertEnd(30));
    ASSERT_TRUE(original.removeFirst());

    EXPECT_EQ(values(original), (std::vector<int>{20}));
    EXPECT_EQ(values(copy), (std::vector<int>{99, 20, 30}));
    EXPECT_TRUE(isCircular(original));
    EXPECT_TRUE(isCircular(copy));
}

TEST(CircularLinkedListTest, CopyOfEmptyListIsEmpty)
{
    CircularLinkedList original;
    CircularLinkedList copy(original);

    EXPECT_EQ(copy.count(), 0);
}

TEST(CircularLinkedListTest, CopyAssignmentReplacesValues)
{
    CircularLinkedList source;
    ASSERT_TRUE(source.insertEnd(1));
    ASSERT_TRUE(source.insertEnd(2));

    CircularLinkedList target;
    ASSERT_TRUE(target.insertEnd(7));
    ASSERT_TRUE(target.insertEnd(8));
    ASSERT_TRUE(target.insertEnd(9));

    target = source;

    EXPECT_EQ(values(target), (std::vector<int>{1, 2}));
    EXPECT_EQ(values(source), (std::vector<int>{1, 2}));
    EXPECT_TRUE(isCircular(target));
}

TEST(CircularLinkedListTest, CopyAssignmentToSelfKeepsValues)
{
    CircularLinkedList list;
    ASSERT_TRUE(list.insertEnd(1));
    ASSERT_TRUE(list.insertEnd(2));

    CircularLinkedList &alias = list;
    list = alias;

    EXPECT_EQ(values(list), (std::vector<int>{1, 2}));
    EXPECT_TRUE(isCircular(list));
}

TEST(CircularLinkedListTest, MoveConstructorTakesNodes)
{
    CircularLinkedList original;
    ASSERT_TRUE(original.insertEnd(10));
    ASSERT_TRUE(original.insertEnd(20));

    int *first = original.getFirst();

    CircularLinkedList moved(std::move(original));

    EXPECT_EQ(values(moved), (std::vector<int>{10, 20}));
    EXPECT_EQ(moved.getFirst(), first); // same nodes, nothing copied
    EXPECT_TRUE(isCircular(moved));
    EXPECT_EQ(original.count(), 0);
}

TEST(CircularLinkedListTest, MoveAssignmentReplacesValues)
{
    CircularLinkedList source;
    ASSERT_TRUE(source.insertEnd(1));
    ASSERT_TRUE(source.insertEnd(2));

    CircularLinkedList target(5);

    target = std::move(source);

    EXPECT_EQ(values(target), (std::vector<int>{1, 2}));
    EXPECT_TRUE(isCircular(target));
    EXPECT_EQ(source.count(), 0);
}

TEST(CircularLinkedListTest, MovedFromListIsReusable)
{
    CircularLinkedList source(1);
    CircularLinkedList target(std::move(source));

    ASSERT_TRUE(source.insertEnd(5));

    EXPECT_EQ(values(source), (std::vector<int>{5}));
    EXPECT_TRUE(isCircular(source));
}

// ---------------------------------------------------------
// Mutation through get
// ---------------------------------------------------------

TEST(CircularLinkedListTest, GetAllowsElementMutation)
{
    CircularLinkedList list(10);

    int *value = list.get(0);

    ASSERT_NE(value, nullptr);

    *value = 99;

    EXPECT_EQ(*list.get(0), 99);
}

// ---------------------------------------------------------
// Stress-ish tests
// ---------------------------------------------------------

TEST(CircularLinkedListTest, HandlesManyElements)
{
    CircularLinkedList list;

    constexpr size_t amount = 1000;

    for (size_t i = 0; i < amount; ++i)
        ASSERT_TRUE(list.insertEnd(static_cast<int>(i)));

    ASSERT_EQ(list.count(), amount);
    EXPECT_TRUE(isCircular(list));

    for (size_t i = 0; i < amount; ++i)
    {
        ASSERT_NE(list.get(i), nullptr);
        EXPECT_EQ(*list.get(i), static_cast<int>(i));
    }
}

TEST(CircularLinkedListTest, MixedOperationsMatchVector)
{
    CircularLinkedList list;
    std::vector<int> expected;

    // a fixed sequence of inserts and removes at the start, middle and end
    for (int step = 0; step < 300; ++step)
    {
        size_t size = expected.size();

        switch (step % 6)
        {
        case 0:
            ASSERT_TRUE(list.insertStart(step));
            expected.insert(expected.begin(), step);
            break;
        case 1:
            ASSERT_TRUE(list.insertEnd(step));
            expected.push_back(step);
            break;
        case 2:
            ASSERT_TRUE(list.insertAt(step, size / 2));
            expected.insert(expected.begin() + size / 2, step);
            break;
        case 3:
            if (size > 0)
            {
                ASSERT_TRUE(list.removeFirst());
                expected.erase(expected.begin());
            }
            break;
        case 4:
            ASSERT_TRUE(list.insertEnd(step));
            expected.push_back(step);
            break;
        case 5:
            if (size > 0)
            {
                ASSERT_TRUE(list.removeLast());
                expected.pop_back();
            }
            break;
        }

        ASSERT_EQ(values(list), expected);
        ASSERT_TRUE(isCircular(list));
    }
}
