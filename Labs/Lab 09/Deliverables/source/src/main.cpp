#include <print>
#include "hash_table.hpp"

template <typename T, size_t S>
void search(ArrayHashTable<T, S> &table, const T &target);

int main()
{
    ArrayHashTable<uint16_t, 10> table;

    std::vector<uint16_t> keys = {23, 43, 13, 27, 33, 37};
    for (uint16_t key : keys)
    {
        table.insert(key);
    }

    std::println("Hash Table:");
    table.forEach([](size_t index, const std::optional<uint16_t> &value) { //
        std::print("[{}] -> ", index);
        if (value.has_value())
            std::println("{}", value.value());
        else
            std::println("-");
    });

    search(table, (uint16_t)27);
    search(table, (uint16_t)99);

    return 0;
}

template <typename T, size_t S>
void search(ArrayHashTable<T, S> &table, const T &target)
{
    std::optional<size_t> result = table.search(target);
    if (result.has_value())
        std::println("Key {} found at index {}", target, result.value());
    else
        std::println("Key {} found not found", target);
}
