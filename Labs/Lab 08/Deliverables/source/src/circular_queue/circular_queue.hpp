#pragma once

#include <stdexcept>
#include <optional>
#include <functional>

class CircularQueue
{
    using T = int;
    using ConstT = const T &;

private:
    size_t capacity;
    size_t i_start = 0;
    size_t i_end = 0;

    T *data;

public:
    explicit CircularQueue(size_t capacity) : capacity(capacity)
    {
        if (capacity <= 0)
            throw std::invalid_argument("capacity > 0 : not satisfied");

        data = new T[capacity];
    }

    ~CircularQueue() { delete[] data; }

    size_t size() const { return i_end - i_start; }

    bool enqueue(T value)
    {
        // if overflow
        if (size() >= capacity)
            return false;

        data[i_end % capacity] = value;
        i_end++;
        return true;
    }

    std::optional<T> deque()
    {
        if (size() == 0)
            return std::nullopt;

        T value = data[i_start % capacity];
        i_start++;
        return value;
    }

    // for displaying the entire queue
    void forEach(std::function<void(ConstT)> callable) const
    {
        for (size_t i = i_start; i < i_end; i++)
        {
            ConstT value = data[i % capacity];
            callable(value);
        }
    }
};
