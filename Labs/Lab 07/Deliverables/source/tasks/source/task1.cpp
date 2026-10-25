#include <print>
#include <optional>

#include "stack.hpp"

void highlightStackElement(const std::string &expression, const std::vector<size_t> &indexes);
std::optional<char> equivalantBracket(char current);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::println("Pass in the expression as the first argument when running");
        return -1;
    }

    std::string expression = argv[1];
    Stack<std::pair<size_t, char>> brackets_pair;

    auto handleClosingBracket = [&brackets_pair, &expression](size_t index, char openingBracket) -> void { //
        if (!brackets_pair.isEmpty() && brackets_pair.peek()->second == openingBracket)
            brackets_pair.pop();
        else
        {
            std::println("Mismatch at idx:{}; Expected {} or new scope.", index, equivalantBracket(brackets_pair.peek()->second).value());
            highlightStackElement(expression, {index});
        }
    };

    for (size_t i = 0; i < expression.size(); i++)
    {
        const char &c = expression[i];

        switch (c)
        {
        case '(':
        case '{':
        case '[':
            brackets_pair.push(std::make_pair(i, c));
            break;

        case ')':
        case '}':
        case ']':
            handleClosingBracket(i, equivalantBracket(c).value_or(' '));
            break;

        // skip any other character
        default:
            continue;
        }
    }

    if (brackets_pair.isEmpty())
        std::println("Valid expression");
    else
    {
        std::println("\nInvalid expression:");
        std::vector<size_t> indexes;

        for (const auto &[index, bracket] : brackets_pair)
            indexes.push_back(index);

        highlightStackElement(expression, indexes);

        for (int i = indexes.size(); i > 0; i--)
        {
            size_t index = indexes[i - 1];
            std::println("idx:{}; No valid closing equivalant found for {}", index, expression[index]);
        }
    }

    return 0;
}

void highlightStackElement(const std::string &expression, const std::vector<size_t> &indexes)
{
    std::println("{}", expression);

    std::string markers(expression.size(), ' ');

    for (size_t index : indexes)
        if (index < markers.size())
            markers[index] = '^';

    std::println("{}", markers);
}

std::optional<char> equivalantBracket(char current)
{
    switch (current)
    {
    case '(':
        return ')';
    case ')':
        return '(';
    case '{':
        return '}';
    case '}':
        return '{';
    case '[':
        return ']';
    case ']':
        return '[';
    default:
        return std::nullopt;
    }
}
