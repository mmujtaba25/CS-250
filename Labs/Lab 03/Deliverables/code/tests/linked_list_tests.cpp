#include <gtest/gtest.h>

#include "linked_list.hpp"

// ---------------------------------------------------------
// Construction / count
// ---------------------------------------------------------

TEST(LinkedListTest, DefaultConstructorCreatesEmptyList)
{
    LinkedList list;

    EXPECT_EQ(list.count(), 0);
    EXPECT_EQ(list.getFirst(), nullptr);
    EXPECT_EQ(list.getLast(), nullptr);
}

TEST(LinkedListTest, ValueConstructorCreatesSingleElement)
{
    LinkedList list(42);

    ASSERT_EQ(list.count(), 1);

    ASSERT_NE(list.get(0), nullptr);
    EXPECT_EQ(*list.get(0), 42);
}

TEST(LinkedListTest, CountTracksNumberOfElements)
{
    LinkedList list;

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

TEST(LinkedListTest, GetReturnsCorrectElements)
{
    LinkedList list;

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

TEST(LinkedListTest, GetReturnsNullForOutOfBoundsIndex)
{
    LinkedList list;

    EXPECT_EQ(list.get(0), nullptr);

    ASSERT_TRUE(list.insertEnd(10));

    EXPECT_EQ(list.get(1), nullptr);
    EXPECT_EQ(list.get(100), nullptr);
}

TEST(LinkedListTest, GetFirstReturnsFirstElement)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_NE(list.getFirst(), nullptr);
    EXPECT_EQ(*list.getFirst(), 10);
}

TEST(LinkedListTest, GetLastReturnsLastElement)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_NE(list.getLast(), nullptr);
    EXPECT_EQ(*list.getLast(), 30);
}

// ---------------------------------------------------------
// insertStart
// ---------------------------------------------------------

TEST(LinkedListTest, InsertStartWorksOnEmptyList)
{
    LinkedList list;

    ASSERT_TRUE(list.insertStart(10));

    ASSERT_EQ(list.count(), 1);
    ASSERT_NE(list.get(0), nullptr);

    EXPECT_EQ(*list.get(0), 10);
}

TEST(LinkedListTest, InsertStartMovesExistingElementsRight)
{
    LinkedList list;

    ASSERT_TRUE(list.insertStart(20));
    ASSERT_TRUE(list.insertStart(10));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
}

// ---------------------------------------------------------
// insertEnd
// ---------------------------------------------------------

TEST(LinkedListTest, InsertEndWorksOnEmptyList)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.get(0), 10);
}

TEST(LinkedListTest, InsertEndAppendsValues)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_EQ(*list.get(2), 30);
}

// ---------------------------------------------------------
// insertAt
// ---------------------------------------------------------

TEST(LinkedListTest, InsertAtBeginning)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.insertAt(10, 0));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_EQ(*list.get(2), 30);
}

TEST(LinkedListTest, InsertAtMiddle)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.insertAt(20, 1));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
    EXPECT_EQ(*list.get(2), 30);
}

TEST(LinkedListTest, InsertAtEnd)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_TRUE(list.insertAt(30, 2));

    ASSERT_EQ(list.count(), 3);

    EXPECT_EQ(*list.get(2), 30);
}

TEST(LinkedListTest, InsertAtZeroWorksOnEmptyList)
{
    LinkedList list;

    ASSERT_TRUE(list.insertAt(10, 0));

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.get(0), 10);
}

TEST(LinkedListTest, InsertAtRejectsInvalidIndex)
{
    LinkedList list;

    EXPECT_FALSE(list.insertAt(10, 1));

    ASSERT_TRUE(list.insertAt(10, 0));

    EXPECT_FALSE(list.insertAt(30, 2));
}

// ---------------------------------------------------------
// removeAt
// ---------------------------------------------------------

TEST(LinkedListTest, RemoveAtBeginning)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeAt(0));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 20);
    EXPECT_EQ(*list.get(1), 30);
}

TEST(LinkedListTest, RemoveAtMiddle)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeAt(1));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 30);
}

TEST(LinkedListTest, RemoveAtEnd)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeAt(2));

    ASSERT_EQ(list.count(), 2);

    EXPECT_EQ(*list.get(0), 10);
    EXPECT_EQ(*list.get(1), 20);
}

TEST(LinkedListTest, RemoveAtOnlyElement)
{
    LinkedList list(10);

    ASSERT_TRUE(list.removeAt(0));

    EXPECT_EQ(list.count(), 0);
    EXPECT_EQ(list.getFirst(), nullptr);
}

TEST(LinkedListTest, RemoveAtRejectsInvalidIndex)
{
    LinkedList list;

    EXPECT_FALSE(list.removeAt(0));

    ASSERT_TRUE(list.insertEnd(10));

    EXPECT_FALSE(list.removeAt(1));
    EXPECT_FALSE(list.removeAt(100));
}

// ---------------------------------------------------------
// removeFirst / removeLast
// ---------------------------------------------------------

TEST(LinkedListTest, RemoveFirstRemovesHead)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_TRUE(list.removeFirst());

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.getFirst(), 20);
}

TEST(LinkedListTest, RemoveLastRemovesTail)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    ASSERT_TRUE(list.removeLast());

    ASSERT_EQ(list.count(), 2);
    EXPECT_EQ(*list.getLast(), 20);
}

TEST(LinkedListTest, RemoveFirstOnEmptyListReturnsFalse)
{
    LinkedList list;

    EXPECT_FALSE(list.removeFirst());
}

TEST(LinkedListTest, RemoveLastOnEmptyListReturnsFalse)
{
    LinkedList list;

    EXPECT_FALSE(list.removeLast());
}

// ---------------------------------------------------------
// removeIf
// ---------------------------------------------------------

TEST(LinkedListTest, RemoveIfRemovesMatchingElement)
{
    LinkedList list;

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
}

TEST(LinkedListTest, RemoveIfCanRemoveHead)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));

    ASSERT_TRUE(list.removeIf(
        [](const int &value, size_t)
        {
            return value == 10;
        }));

    ASSERT_EQ(list.count(), 1);
    EXPECT_EQ(*list.get(0), 20);
}

TEST(LinkedListTest, RemoveIfCanRemoveLast)
{
    LinkedList list;

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
}

TEST(LinkedListTest, RemoveIfUsesCorrectIndex)
{
    LinkedList list;

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

TEST(LinkedListTest, RemoveIfReturnsFalseWhenNothingMatches)
{
    LinkedList list;

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

// ---------------------------------------------------------
// forEach
// ---------------------------------------------------------

TEST(LinkedListTest, ConstForEachVisitsEveryElementInOrder)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    const LinkedList &constList = list;

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

TEST(LinkedListTest, ForEachProvidesCorrectIndexes)
{
    LinkedList list;

    ASSERT_TRUE(list.insertEnd(10));
    ASSERT_TRUE(list.insertEnd(20));
    ASSERT_TRUE(list.insertEnd(30));

    const LinkedList &constList = list;

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

// ---------------------------------------------------------
// Mutation through get
// ---------------------------------------------------------

TEST(LinkedListTest, GetAllowsElementMutation)
{
    LinkedList list(10);

    int *value = list.get(0);

    ASSERT_NE(value, nullptr);

    *value = 99;

    EXPECT_EQ(*list.get(0), 99);
}

// ---------------------------------------------------------
// Stress-ish test
// ---------------------------------------------------------

TEST(LinkedListTest, HandlesManyElements)
{
    LinkedList list;

    constexpr size_t amount = 1000;

    for (size_t i = 0; i < amount; ++i)
        ASSERT_TRUE(list.insertEnd(static_cast<int>(i)));

    ASSERT_EQ(list.count(), amount);

    for (size_t i = 0; i < amount; ++i)
    {
        ASSERT_NE(list.get(i), nullptr);
        EXPECT_EQ(*list.get(i), static_cast<int>(i));
    }
}
