#include <print>
#include <fstream>
#include <sstream>
#include <optional>
#include <expected>
#include <cctype>
#include <charconv>

#include "stack.hpp"

std::vector<std::string> readCSV(std::string filename);
std::vector<std::string> infixToPostfix(const std::vector<std::string> &tokens);
double evaluatePostfix(const std::vector<std::string> &tokens);

// UTILS
bool isOperator(const std::string &token);
std::expected<double, std::errc> isNumber(const std::string &token);
int precedence(const std::string &op);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::println("Pass in the filename as the first argument when running");
        return -1;
    }

    std::string infixFile = argv[1];

    std::vector<std::string> infix = readCSV(infixFile);

    std::print("Expression : ");
    for (const std::string &token : infix)
    {
        std::print("{} ", token);
    }
    std::println();

    std::vector<std::string> tokens = infixToPostfix(infix);

    std::print("Converted  : ");
    for (const std::string &token : tokens)
    {
        std::print("{} ", token);
    }
    std::print("= {}\n", evaluatePostfix(tokens));

    return 0;
}

std::vector<std::string> readCSV(std::string filename)
{
    std::fstream infixFile(filename);
    std::vector<std::string> tokens(0);

    if (!infixFile.is_open())
    {
        std::println("Error opening CSV file");
        return {};
    }

    std::string line;
    while (std::getline(infixFile, line))
    {
        // convert string to a stream
        std::stringstream ss(line);

        // split by ,
        std::string field;
        while (std::getline(ss, field, ','))
            tokens.push_back(field);
    }

    infixFile.close();

    return tokens;
}

std::vector<std::string> infixToPostfix(const std::vector<std::string> &tokens)
{
    Stack<std::string> stack;
    std::vector<std::string> postfix;

    for (const std::string &token : tokens)
    {
        if (isNumber(token))
            postfix.push_back(token);
        else if (token.size() == 1 && token[0] == '(')
            stack.push(token);
        else if (token.size() == 1 && token[0] == ')')
        {
            // while stack not empty && not (stack top is "(")
            while (!stack.isEmpty() && !(stack.peek()->size() == 1 && (*stack.peek())[0] == '('))
            {
                postfix.push_back(stack.pop().value());
            }
            stack.pop();
        }
        else if (isOperator(token))
        {
            // while stack not empty && not (stack top is "(") && precendence of next is greater then of current
            while (!stack.isEmpty()                                             //
                   && !(stack.peek()->size() == 1 && (*stack.peek())[0] == '(') //
                   && (precedence(*stack.peek()) >= precedence(token)))
            {
                postfix.push_back(stack.pop().value());
            }
            stack.push(token);
        }
    }

    while (!stack.isEmpty())
        postfix.push_back(stack.pop().value());

    return postfix;
}

double evaluatePostfix(const std::vector<std::string> &tokens)
{
    Stack<double> numbers;

    for (const std::string &token : tokens)
    {
        if (auto num = isNumber(token))
            numbers.push(*num);
        else if (isOperator(token))
        {
            // pop right operand first
            double b = numbers.pop().value();
            double a = numbers.pop().value();
            double result;

            switch (token[0])
            {
            case '+':
                result = a + b;
                break;

            case '-':
                result = a - b;
                break;

            case '*':
                result = a * b;
                break;

            case '/':
                result = a / b;
                break;
            }

            numbers.push(result);
        }
    }

    return numbers.pop().value();
}

// MARK: UTILS

bool isOperator(const std::string &token)
{
    if (token.size() != 1)
        return false;

    switch (token[0])
    {
    case '+':
    case '-':
    case '*':
    case '/':
        return true;

    default:
        return false;
    }
}

std::expected<double, std::errc> isNumber(const std::string &token)
{
    if (token.empty())
        return std::unexpected(std::errc::invalid_argument);

    double value;
    // attempt to parse as double
    auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), value);

    // propogate error
    if (ec != std::errc{})
        return std::unexpected(ec);

    // check if fully parsed; fail "123a"
    if (ptr != token.data() + token.size())
        return std::unexpected(std::errc::invalid_argument);

    return value;
}

int precedence(const std::string &op)
{
    if (!isOperator(op))
        return 0;

    switch (op[0])
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;

    default:
        return 0;
    }
}
