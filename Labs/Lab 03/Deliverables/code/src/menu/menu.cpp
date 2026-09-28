#include "menu.hpp"

#include <functional>
#include <iostream>
#include <sstream>
#include <random>
#include <charconv>
#include <print>

void ListTester::start(std::function<bool()> shouldStop)
{
    printMainMenuOptions();

    bool running = true;
    while (running)
    {
        std::string user_input = getUserInputClean("Main Menu");
        MainMenuInput input = parseMainInput(user_input);

        std::println();

        switch (input)
        {
        case MainMenuInput::DISPLAY:
            display();
            break;

        case MainMenuInput::INSERT:
            insert(InsertInput::parse(getUserInputClean("Insert Element (<value>:<optional:index=end>)")));
            display();
            break;

        case MainMenuInput::REMOVE:
            display();
            remove(RemoveInput::parse(getUserInputClean("Remove Element (<value> OR :<index=end>)")));
            display();
            break;

        case MainMenuInput::EXIT:
            std::println("Exiting . . . ");
            running = false;
            break;

        case MainMenuInput::INVALID_INPUT:
        default:
            std::println("Invalid Input . . . ");
            printMainMenuOptions();
            break;
        }

        std::println();

        if (shouldStop != nullptr)
            running = !shouldStop();
    }
}

// MARK: Functions

void ListTester::display()
{
    std::print("LIST: {}", CHOSEN_LIST_START_CHAR);
    size_t numElems = list.count();
    list.forEach([&numElems](ConstNode node, size_t index) { //
        std::print("{}{}", node->data,                       // data
                   (index == (numElems - 1) ? "" : " ")      // space if not last element
        );
    });
    std::println("{}", CHOSEN_LIST_END_CHAR);
}

void ListTester::insert(ListTester::InsertInput input)
{
    bool success = false;

    if (input.index == -1)
        success = list.insertEnd(input.value);
    else if (input.index >= 0)
        success = list.insertAt(input.value, input.index);

    if (success)
        std::println("Insertion of \"{}\" Succesful . . .", input.value);
    else
        std::println("Insertion of \"{}\" Failed . . .", input.value);
}

void ListTester::remove(ListTester::RemoveInput input)
{
    bool success = false;

    // index based selection
    if (input.isIndex)
    {
        if (input.input < 0)
            success = list.removeLast();
        else
            success = list.removeAt(input.input);
    }
    else
    {
        success = list.removeIf([&input](const int &value, size_t index) { //
            return value == input.input;
        });
    }

    if (success)
        std::println("Removal of \"{}{}\" Succesful . . .", (input.isIndex ? "index: " : ""), input.input);
    else
        std::println("Removal of \"{}{}\" Failed . . .", (input.isIndex ? "index: " : ""), input.input);
}

// MARK: Parsing

/// @brief
/// DISPLAY = 0,
///
/// INSERT = 1,
///
/// REMOVE = 2,
///
/// EXIT = 3,
///
/// @param input cleaned user input
/// @return
ListTester::MainMenuInput ListTester::parseMainInput(const std::string &input)
{
    std::string result;
    int parsed;

    if (!tryParseInt(input, &parsed))
        return MainMenuInput::INVALID_INPUT;

    // valid values: 0 to 3
    if (parsed < static_cast<int>(MainMenuInput::DISPLAY) || parsed > static_cast<int>(MainMenuInput::EXIT))
        return MainMenuInput::INVALID_INPUT;

    return static_cast<MainMenuInput>(parsed);
}

ListTester::InsertInput ListTester::InsertInput::parse(const std::string &inputStr)
{
    // seperate by :
    std::vector<std::string> split = ListTester::split(inputStr, ':');
    size_t split_size = split.size();
    // either none or more than 2
    if (split_size == 0 || split_size > 2)
        return InsertInput::Invalid();

    // parse the ints
    int value;
    int index;

    // FORM: <value>
    if (split_size == 1)
    {
        // -1 for insertion at the end
        index = -1;

        // if cannot parse return invalid
        if (!tryParseInt(split[0], &value))
        {
            return InsertInput::Invalid();
        }
    }
    // FORM: <value>:<optional:index=end>
    else if (split_size == 2)
    {
        if (!tryParseInt(split[0], &value))
            // "value" MUST be parsed
            return InsertInput::Invalid();

        // FORM: <value>:
        if (split[1].empty())
            index = -1;
        // FORM: <value>:ABC
        else if (!tryParseInt(split[1], &index))
            index = -1; // if failed to parse
    }

    // return the result
    return InsertInput{.value = value, .index = index, .invalid = false};
}

ListTester::RemoveInput ListTester::RemoveInput::parse(const std::string &inputStr)
{
    // seperate by :
    std::vector<std::string> split = ListTester::split(inputStr, ':');
    size_t split_size = split.size();
    // either none or more than 2
    if (split_size == 0 || split_size > 2) // split_size will be 2 with split[0] empty for :<value>
        return RemoveInput::Invalid();

    // parse
    int input = 0;
    bool isIndex = false;

    // FORM: <value>
    if (split_size == 1)
    {
        isIndex = false; // no ":" exists
        // if cannot parse return invalid
        if (!tryParseInt(split[0], &input))
            return RemoveInput::Invalid();
    }
    // FORM: :<index=end>
    else if (split_size == 2)
    {
        // nothing before ":", if ":" present (true here since split_size == 2)
        if (!split[0].empty())
            return RemoveInput::Invalid();

        // since ":" exists
        isIndex = true;

        // FORM: :
        if (split[1].empty())
            input = -1; // remove last
        // FORM: :<value>
        else if (!tryParseInt(split[1], &input))
            return RemoveInput::Invalid();
    }

    // return the result
    return RemoveInput{.input = input, .isIndex = isIndex, .invalid = false};
}

// MARK: Helpers

ChosenList ListTester::createTestList()
{
    constexpr int SEED = 0;
    ChosenList list{};

    for (size_t i = 0; i < 10; i++)
    {
        list.insertEnd(getRandomInt(0, 10, SEED));
    }

    return list;
}

int ListTester::getRandomInt(int minInc, int maxInc, std::optional<int> seed)
{
    std::mt19937 gen{seed.has_value() ? static_cast<unsigned int>(*seed) : std::random_device{}()};
    std::uniform_int_distribution<int> distr(minInc, maxInc);
    return distr(gen);
}

std::string ListTester::getUserInputClean(const std::string &helpText)
{
    std::string input;
    std::print("{}> ", helpText);
    std::getline(std::cin, input);
    return ListTester::trim(input);
}

std::string ListTester::trim(const std::string &str)
{
    // find first character not [space, tab ...]
    const auto start = str.find_first_not_of(" \t\n\r");
    // if not found, means string has no displayable characters
    if (start == std::string::npos)
        return "";

    // find last character not [space, tab ...]
    const auto end = str.find_last_not_of(" \t\n\r");
    // sub string to include all between first and last space
    return str.substr(start, end - start + 1);
}

std::vector<std::string> ListTester::split(const std::string &str, char seperator)
{
    std::vector<std::string> result;

    // convert to stream
    std::stringstream stream(str);

    // seperate part by part and add to vector
    std::string part;
    while (std::getline(stream, part, seperator))
        result.push_back(part);

    return result;
}

bool ListTester::tryParseInt(const std::string &str, int *value)
{
    // stop_point : where parsing stopped

    const auto [stop_point, error_code] = std::from_chars( //
        /* string start */ str.data(),
        /* stirng end   */ str.data() + str.size(),
        /* result       */ *value //
    );

    // errc is struct of standard errors such as std::errc::invalid_argument && std::errc::result_out_of_range
    // errc{} correspoinds to no error

    return error_code == std::errc{}                 // did error occur?
           && stop_point == str.data() + str.size(); // did parse completely?
}

// MARK: Print Helpers

void ListTester::printMainMenuOptions()
{
    std::println("\n\n --- Main Menu --- ");
    std::println(CHOSEN_LIST_NAME);
    std::println("0. Display ");
    std::println("1. Insert ");
    std::println("2. Remove ");
    std::println("3. Exit ");
}
