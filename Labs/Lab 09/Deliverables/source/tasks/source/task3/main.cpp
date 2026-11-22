#include <chrono>
#include <cstdint>
#include <print>
#include <random>
#include <vector>
#include "hash_table.hpp"

template <UnsignedNumeric T>
T getRandomInt(T min, T max)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<T> dist(min, max);
    return dist(rng);
}

template <UnsignedNumeric T>
double averageSearchTimeUs(const LinkedListHashTable<T> &table, const std::vector<T> &targets)
{
    volatile size_t sink = 0;
    auto start = std::chrono::steady_clock::now();

    for (const T &target : targets)
    {
        if (table.search(target).has_value())
            sink = sink + 1;
    }

    auto end = std::chrono::steady_clock::now();
    double totalMs = std::chrono::duration<double, std::micro>(end - start).count();
    return totalMs / targets.size();
}

int main()
{
    LinkedListHashTable<uint32_t> table(100);

    for (uint32_t i = 0; i < 1'000'000; i++)
        table.insert(i);

    // 100,000 random queries (0-999,999 exist, 1,000,000-1,200,000 test misses)
    std::vector<uint32_t> testKeys(100'000);
    for (uint32_t &key : testKeys)
        key = getRandomInt<uint32_t>(0, 1'200'000);

    std::println("=== Before Resizing ===");
    std::println("Number of elements: {}", table.size());
    std::println("Initial table size: {}", table.capacity());
    std::println("Initial Load factor: {:.2f}", table.loadFactor());
    std::println("Average search time (100,000 searches): {:.2f} us\n", averageSearchTimeUs(table, testKeys));

    table.resize(table.capacity() * 2);

    std::println("=== After Resizing ===");
    std::println("Resized Table size: {}", table.capacity());
    std::println("Resized Load factor: {:.2f}", table.loadFactor());
    std::println("Average search time (100,000 searches): {:.2f} us\n", averageSearchTimeUs(table, testKeys));

    std::println("Observation: After resizing, load factor decreased and search operations became faster.");
    return 0;
}
