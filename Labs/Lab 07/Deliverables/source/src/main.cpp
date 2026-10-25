#include <print>
#include <charconv>
#include <limits>
#include <string>
#include <string_view>
#include <utility>
#include <expected>
#include <cstdint>

#include "stack.hpp"

struct Rod
{
    Stack<uint8_t> disks{};
    char name;
};

void makeLegalMove(Rod &x, Rod &y);
void printRod(const Rod &rod, char name);

void printMoveDisk(int disk, char source, char destination)
{
    std::println("Move {} from {} to {}", disk, source, destination);
}

std::expected<double, std::errc> isNumber(const std::string &str);

void towerOfHanoi(int numDisks, Rod &rodA, Rod &rodB, Rod &rodC);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::println("Pass in the number of disks as the first argument");
        return 1;
    }

    auto result = isNumber(argv[1]);

    if (!result || *result < 1 || *result > 63 ||
        *result != static_cast<int>(*result))
    {
        std::println("Enter a positive integer between 1 and 63");
        return 1;
    }

    int numDisks = static_cast<int>(*result);

    Rod rodA{.name = 'A'};
    Rod rodB{.name = 'B'};
    Rod rodC{.name = 'C'};

    // largest disk at the bottom
    for (uint8_t disk = numDisks; disk >= 1; --disk)
        rodA.disks.push(disk);

    towerOfHanoi(numDisks, rodA, rodB, rodC);

    std::println();
    printRod(rodA, rodA.name);
    printRod(rodB, rodB.name);
    printRod(rodC, rodC.name);

    return 0;
}

void towerOfHanoi(int numDisks, Rod &rodA, Rod &rodB, Rod &rodC)
{
    // preserve the original rod identities
    Rod *A = &rodA;
    Rod *B = &rodB;
    Rod *C = &rodC;

    // even number of disks; swap the roles of B and C
    if (numDisks % 2 == 0)
        std::swap(B, C);

    uint64_t totalMoves = (uint64_t{1} << numDisks) - 1;
    for (uint64_t i = 1; i <= totalMoves; ++i)
    {
        if (i % 3 == 1)
            makeLegalMove(*A, *C);
        else if (i % 3 == 2)
            makeLegalMove(*A, *B);
        else
            makeLegalMove(*B, *C);
    }
}

void makeLegalMove(Rod &x, Rod &y)
{
    // if X empty; move from Y to X.
    if (x.disks.isEmpty())
    {
        uint8_t disk = y.disks.pop().value();
        x.disks.push(disk);
        printMoveDisk(disk, y.name, x.name);
        return;
    }

    // if Y empty; move from X to Y.
    if (y.disks.isEmpty())
    {
        uint8_t disk = x.disks.pop().value();
        y.disks.push(disk);
        printMoveDisk(disk, x.name, y.name);
        return;
    }

    // inspect both top disks
    uint8_t diskX = *x.disks.peek();
    uint8_t diskY = *y.disks.peek();

    // move the smaller disk onto the larger disk.
    if (diskX < diskY)
    {
        x.disks.pop();
        y.disks.push(diskX);
        printMoveDisk(diskX, x.name, y.name);
    }
    else
    {
        y.disks.pop();
        x.disks.push(diskY);
        printMoveDisk(diskY, y.name, x.name);
    }
}

void printRod(const Rod &rod, char name)
{
    std::print("Rod {} : ", name);

    if (rod.disks.isEmpty())
        std::print("(empty)");
    else
        for (const int disk : rod.disks)
            std::print("{} ", disk);

    std::println();
}

std::expected<double, std::errc> isNumber(const std::string &str)
{
    if (str.empty())
        return std::unexpected(std::errc::invalid_argument);

    double value;
    // attempt to parse as double
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);

    // propogate error
    if (ec != std::errc{})
        return std::unexpected(ec);

    // check if fully parsed; fail "123a"
    if (ptr != str.data() + str.size())
        return std::unexpected(std::errc::invalid_argument);

    return value;
}
