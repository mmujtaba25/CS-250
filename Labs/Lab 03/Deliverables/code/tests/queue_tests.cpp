#include <gtest/gtest.h>

#include <vector>

#include "queue.hpp"

// ---------------------------------------------------------
// Construction
// ---------------------------------------------------------

TEST(QueueTest, DefaultConstructorCreatesEmptyQueue)
{
    Queue queue;

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    EXPECT_TRUE(values.empty());
}

// ---------------------------------------------------------
// enqueue
// ---------------------------------------------------------

TEST(QueueTest, EnqueueWorks)
{
    Queue queue;

    EXPECT_TRUE(queue.enqueue(10));

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    ASSERT_EQ(values.size(), 1);
    EXPECT_EQ(values[0], 10);
}

TEST(QueueTest, EnqueuePreservesFIFOOrder)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    ASSERT_EQ(values.size(), 3);

    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

TEST(QueueTest, MultipleEnqueuesPreserveOrder)
{
    Queue queue;

    for (int i = 0; i < 100; ++i)
        ASSERT_TRUE(queue.enqueue(i));

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    ASSERT_EQ(values.size(), 100);

    for (size_t i = 0; i < values.size(); ++i)
        EXPECT_EQ(values[i], static_cast<int>(i));
}

// ---------------------------------------------------------
// peek
// ---------------------------------------------------------

TEST(QueueTest, PeekReturnsFrontElement)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    EXPECT_EQ(queue.peek(), 10);
}

TEST(QueueTest, PeekDoesNotRemoveElement)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));

    EXPECT_EQ(queue.peek(), 10);
    EXPECT_EQ(queue.peek(), 10);

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    ASSERT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
}

TEST(QueueTest, PeekChangesAfterDequeue)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    EXPECT_EQ(queue.peek(), 10);

    EXPECT_EQ(queue.dequeue(), 10);
    EXPECT_EQ(queue.peek(), 20);

    EXPECT_EQ(queue.dequeue(), 20);
    EXPECT_EQ(queue.peek(), 30);
}

// ---------------------------------------------------------
// dequeue
// ---------------------------------------------------------

TEST(QueueTest, DequeueReturnsFrontElement)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));

    EXPECT_EQ(queue.dequeue(), 10);
}

TEST(QueueTest, DequeueRemovesFrontElement)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    EXPECT_EQ(queue.dequeue(), 10);

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    ASSERT_EQ(values.size(), 2);

    EXPECT_EQ(values[0], 20);
    EXPECT_EQ(values[1], 30);
}

TEST(QueueTest, DequeueFollowsFIFOOrder)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    EXPECT_EQ(queue.dequeue(), 10);
    EXPECT_EQ(queue.dequeue(), 20);
    EXPECT_EQ(queue.dequeue(), 30);
}

TEST(QueueTest, DequeueAndEnqueueMaintainFIFOOrder)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));

    EXPECT_EQ(queue.dequeue(), 10);

    ASSERT_TRUE(queue.enqueue(30));

    EXPECT_EQ(queue.dequeue(), 20);
    EXPECT_EQ(queue.dequeue(), 30);
}

// ---------------------------------------------------------
// Single element
// ---------------------------------------------------------

TEST(QueueTest, SingleElementCanBeEnqueuedAndDequeued)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(42));

    EXPECT_EQ(queue.peek(), 42);
    EXPECT_EQ(queue.dequeue(), 42);

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    EXPECT_TRUE(values.empty());
}

TEST(QueueTest, QueueCanBeReusedAfterBecomingEmpty)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    EXPECT_EQ(queue.dequeue(), 10);

    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    EXPECT_EQ(queue.peek(), 20);
    EXPECT_EQ(queue.dequeue(), 20);
    EXPECT_EQ(queue.dequeue(), 30);
}

// ---------------------------------------------------------
// forEach
// ---------------------------------------------------------

TEST(QueueTest, ForEachVisitsEveryElementInQueueOrder)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    std::vector<int> values;

    queue.forEach(
        [&values](ConstNode node, size_t)
        {
            values.push_back(node->data);
        });

    ASSERT_EQ(values.size(), 3);

    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

TEST(QueueTest, ForEachProvidesCorrectIndexes)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    std::vector<size_t> indexes;

    queue.forEach(
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
// Mixed operations
// ---------------------------------------------------------

TEST(QueueTest, MixedOperationsPreserveFIFOBehavior)
{
    Queue queue;

    ASSERT_TRUE(queue.enqueue(10));
    ASSERT_TRUE(queue.enqueue(20));
    ASSERT_TRUE(queue.enqueue(30));

    EXPECT_EQ(queue.dequeue(), 10);

    ASSERT_TRUE(queue.enqueue(40));

    EXPECT_EQ(queue.peek(), 20);
    EXPECT_EQ(queue.dequeue(), 20);

    ASSERT_TRUE(queue.enqueue(50));

    EXPECT_EQ(queue.dequeue(), 30);
    EXPECT_EQ(queue.dequeue(), 40);
    EXPECT_EQ(queue.dequeue(), 50);
}

// ---------------------------------------------------------
// Stress-ish test
// ---------------------------------------------------------

TEST(QueueTest, HandlesManyElements)
{
    Queue queue;

    constexpr size_t amount = 1000;

    for (size_t i = 0; i < amount; ++i)
        ASSERT_TRUE(queue.enqueue(static_cast<int>(i)));

    for (size_t i = 0; i < amount; ++i)
        EXPECT_EQ(queue.dequeue(), static_cast<int>(i));
}
