#include <chrono>
#include <print>

int main()
{
    int n = 5;
    int arr[5] = {1, 2, 3, 4, 5};

    std::chrono::time_point start = std::chrono::steady_clock::now();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            std::print("{}, ", arr[i] + arr[j]);
        std::println();
    }

    std::chrono::time_point end = std::chrono::steady_clock::now();

    auto timeInUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    auto timeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::println("Time For Program: {} ≈ {}", timeInUs, timeInMs);

    return 0;
}
