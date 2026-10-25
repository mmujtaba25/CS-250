#pragma once

#include <vector>
#include <optional>
#include <utility>
#include <cstddef>

template <typename T>
class Stack
{
private:
    std::vector<T> elements;

public:
    void push(const T &value) { elements.push_back(value); }
    void push(T &&value) { elements.push_back(std::move(value)); }

    std::optional<T> pop()
    {
        if (isEmpty())
            return std::nullopt;

        T value = std::move(elements.back());
        elements.pop_back();
        return value;
    }

    const T *peek() const
    {
        if (isEmpty())
            return nullptr;

        return &elements.back();
    }

    constexpr bool isEmpty() const noexcept { return elements.empty(); }
    constexpr size_t count() const noexcept { return elements.size(); }

    // allow constant range-based for loop (top to bottom)
    using const_iterator = typename std::vector<T>::const_reverse_iterator;
    const_iterator begin() const { return elements.crbegin(); }
    const_iterator end() const { return elements.crend(); }
};
