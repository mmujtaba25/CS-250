#include <cstdint>
#include <print>
#include "hash_table.hpp"

int main()
{
    LinkedListHashTable<uint16_t, 7> table;

    for (uint16_t key : {14, 33, 8, 15, 9, 19, 11, 27, 12})
        table.insert(key);

    table.forEach([](size_t index, const SlotNode<uint16_t> &slot) { //
        std::print("[{}] -> ", index);
        if (!slot.exists())
        {
            std::println("-");
            return;
        }

        for (const auto *curr = slot.getHead(); curr != nullptr; curr = curr->next)
            std::print("{}{}", curr->value, curr->next ? " -> " : "\n");
    });

    return 0;
}
