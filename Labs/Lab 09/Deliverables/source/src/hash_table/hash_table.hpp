#pragma once

#include <functional>
#include <optional>
#include <concepts>

template <typename T>
concept UnsignedNumeric = // allow only unsigned since -3 % x is < 0 (out of bounds)
    std::unsigned_integral<T> && !std::same_as<T, bool> &&
    !std::same_as<T, char8_t> && !std::same_as<T, char16_t> &&
    !std::same_as<T, char32_t> && !std::same_as<T, wchar_t>;

template <UnsignedNumeric T, size_t S = 10>
    requires(S > 0)
class ArrayHashTable
{
private: // MARK: Meta
    class Slot
    {
    private:
        enum class SlotState
        {
            UNASSIGNED,
            OCCUPIED,
            DELETED
        };

        T _value;
        SlotState state = SlotState::UNASSIGNED;

    public:
        bool exists() const { return state == SlotState::OCCUPIED; }
        bool unassigned() const { return state == SlotState::UNASSIGNED; }

        std::optional<T> get() const
        {
            if (!exists())
                return std::nullopt;

            return _value;
        }

        // new_value = nullptr; when empty
        void set(const T *new_value)
        {
            if (new_value)
            {
                _value = *new_value;
                state = SlotState::OCCUPIED;
            }
            else
            {
                // empty state is only for default, otherwise always deleted
                // _value can be anything, we dont care, it is only get() if OCCUPIED
                state = SlotState::DELETED;
            }
        }
    };

private: // MARK: Hash Source
    Slot *data;

    size_t hash(const T &element) const { return element % S; }

public:
    ArrayHashTable() { data = new Slot[S]; }
    ~ArrayHashTable() { delete[] data; }

    // copying
    ArrayHashTable(const ArrayHashTable &other)
    {
        data = new Slot[S];
        for (size_t i = 0; i < S; i++)
            data[i] = other.data[i];
    }

    ArrayHashTable &operator=(const ArrayHashTable &other)
    {
        if (this == &other)
            return *this;

        for (size_t i = 0; i < S; i++)
            data[i] = other.data[i];

        return *this;
    }

    bool insert(const T &newValue)
    {
        size_t insert_index = hash(newValue);

        // try to insert at all between [insert_index ... insert_index - 1] (wrapped)
        for (size_t i = 0; i < S; i++) // loop S times
        {
            size_t query_i = (insert_index + i) % S;

            if (data[query_i].exists())
                continue;

            // empty element
            data[query_i].set(&newValue);
            return true;
        }

        return false;
    }

    bool remove(const T &element)
    {
        std::optional<size_t> index = search(element);
        if (!index.has_value())
            return false;

        data[index.value()].set(nullptr);
        return true;
    }

    std::optional<size_t> search(const T &element) const
    {
        size_t hashed_index = hash(element);

        // check from hashed_index to [first_unassigned_slot OR entire_array]
        for (size_t i = 0; i < S; i++) // loop S times
        {
            size_t query_i = (hashed_index + i) % S;

            if (data[query_i].unassigned())
                return std::nullopt;

            // data[query_i].exists() : to ignore DELETED
            if (data[query_i].exists() && data[query_i].get().value() == element)
                return query_i;
        }

        return std::nullopt;
    }

    // immutable for each
    void forEach(std::function<void(size_t, const std::optional<T> &)> callable) const
    {
        for (size_t i = 0; i < S; i++)
            callable(i, data[i].get());
    }
};
