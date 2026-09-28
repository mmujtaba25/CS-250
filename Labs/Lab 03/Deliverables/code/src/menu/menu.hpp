#pragma once

#include <vector>
#include <string>
#include <optional>

// MARK: Chose the List to test here
#if 0 // switch between 0 and 1
#include "circular_linked_list.hpp"
using ChosenList = CircularLinkedList;
constexpr const char CHOSEN_LIST_START_CHAR = '(';
constexpr const char CHOSEN_LIST_END_CHAR = ')';
constexpr const char *CHOSEN_LIST_NAME = "Circular Linked List";
#else
#include "linked_list.hpp"
using ChosenList = LinkedList;
constexpr const char CHOSEN_LIST_START_CHAR = '[';
constexpr const char CHOSEN_LIST_END_CHAR = ']';
constexpr const char *CHOSEN_LIST_NAME = "Singly Linked List";
#endif

class ListTester
{
    // forward declerations
    struct InsertInput;
    struct RemoveInput;

public:
    /// @brief Start the main menu process in the terminal
    /// @param shouldStop Predicate called at the end of each frame/iteration,
    /// if true halts the execution
    void start(std::function<bool()> shouldStop = nullptr);

private:
    ChosenList list;

    // functions
    void display();
    void insert(InsertInput input);
    void remove(RemoveInput input);

    // inputs
    enum class MainMenuInput : int
    {
        DISPLAY = 0,
        INSERT = 1,
        REMOVE = 2,
        EXIT = 3,
        INVALID_INPUT = -1
    };

    MainMenuInput parseMainInput(const std::string &input);

    struct InsertInput
    {
        int value = 0;
        /// @brief if -1 then insert at end
        int index = -1;

        bool invalid = false;
        static const InsertInput Invalid() { return InsertInput{.invalid = true}; }

        /// @brief Format: `<value>:<optional:index=end>`
        static InsertInput parse(const std::string &inputStr);
    };

    struct RemoveInput
    {
        /// @brief if `isIndex` and `input < 0` => remove last
        int input = 0;
        bool isIndex = false;

        bool invalid = false;
        static const RemoveInput Invalid() { return RemoveInput{.invalid = true}; }

        /// @brief Format:
        /// `<value> OR :<index=end>`
        ///
        /// ("`: [ENTER]`" removes the last element)
        static RemoveInput parse(const std::string &inputStr);
    };

    // helpers
    ChosenList createTestList();
    int getRandomInt(int minInc, int maxInc, std::optional<int> seed = std::nullopt);

    std::string getUserInputClean(const std::string &helpText);
    static std::string trim(const std::string &str);
    static std::vector<std::string> split(const std::string &str, char seperator);
    static bool tryParseInt(const std::string &str, int *value);

    // print helpers

    void printMainMenuOptions();
};
