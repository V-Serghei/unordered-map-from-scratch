#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "unordered_map_from_scratch/unordered_map.hpp"

template<typename Key, typename Data>
void assert_same_map(UnorderedMAP<Key, Data> &actual, const std::unordered_map<Key, Data> &expected) {
    assert(actual.size() == expected.size());

    for (const auto &[key, value]: expected) {
        assert(actual.at(key) == value);
        assert(actual.find(key) != actual.end());
    }
}

template<typename Key, typename Data>
void assert_missing_key(UnorderedMAP<Key, Data> &actual, const Key &key) {
    assert(actual.find(key) == actual.end());

    bool throws = false;
    try {
        actual.at(key);
    }
    catch (const std::out_of_range &) {
        throws = true;
    }

    assert(throws);
}

int main() {
    UnorderedMAP<std::string, std::string> colors = {
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"}};

    std::unordered_map<std::string, std::string> std_colors = {
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"}};

    assert_same_map(colors, std_colors);

    colors["BLACK"] = "#000000";
    std_colors["BLACK"] = "#000000";
    colors["WHITE"] = "#FFFFFF";
    std_colors["WHITE"] = "#FFFFFF";
    assert_same_map(colors, std_colors);

    colors.emplace("RED", "#111111");
    std_colors.emplace("RED", "#111111");
    assert_same_map(colors, std_colors);

    colors.try_emplace("GREEN", "#222222");
    std_colors.try_emplace("GREEN", "#222222");
    assert_same_map(colors, std_colors);

    colors.insert_or_assign("BLUE", "#0000AA");
    std_colors.insert_or_assign("BLUE", "#0000AA");
    assert_same_map(colors, std_colors);

    colors.insert({"YELLOW", "#FFFF00"});
    std_colors.insert({"YELLOW", "#FFFF00"});
    assert_same_map(colors, std_colors);

    colors.insert(colors.begin(), {"PURPLE", "#800080"});
    std_colors.insert(std_colors.begin(), {"PURPLE", "#800080"});
    assert_same_map(colors, std_colors);

    colors.reserve(100);
    std_colors.reserve(100);
    assert_same_map(colors, std_colors);

    UnorderedMAP<std::string, std::string> extra_colors = {
            {"CYAN",    "#00FFFF"},
            {"MAGENTA", "#FF00FF"}};
    std::unordered_map<std::string, std::string> std_extra_colors = {
            {"CYAN",    "#00FFFF"},
            {"MAGENTA", "#FF00FF"}};

    colors.merge(extra_colors);
    std_colors.merge(std_extra_colors);
    assert_same_map(colors, std_colors);

    colors.erase_Key("BLACK");
    std_colors.erase("BLACK");
    assert_same_map(colors, std_colors);
    assert_missing_key(colors, std::string("BLACK"));

    auto erase_it = colors.find("WHITE");
    assert(erase_it != colors.end());
    colors.erase(erase_it);
    std_colors.erase("WHITE");
    assert_same_map(colors, std_colors);
    assert_missing_key(colors, std::string("WHITE"));

    UnorderedMAP<std::string, std::string> colors_copy(colors);
    assert(colors_copy == colors);
    assert_same_map(colors_copy, std_colors);

    UnorderedMAP<std::string, std::string> colors_assigned;
    colors_assigned = colors;
    assert(colors_assigned == colors);
    assert_same_map(colors_assigned, std_colors);

    colors.clear();
    std_colors.clear();
    assert(colors.empty());
    assert_same_map(colors, std_colors);

    colors["AFTER_CLEAR"] = "#123456";
    std_colors["AFTER_CLEAR"] = "#123456";
    assert_same_map(colors, std_colors);

    UnorderedMAP<int, int> numbers = {
            {1, 100},
            {2, 200},
            {3, 300}};
    std::unordered_map<int, int> std_numbers = {
            {1, 100},
            {2, 200},
            {3, 300}};

    numbers.emplace(1, 999);
    std_numbers.emplace(1, 999);
    numbers.try_emplace(2, 999);
    std_numbers.try_emplace(2, 999);
    numbers.insert_or_assign(3, 333);
    std_numbers.insert_or_assign(3, 333);
    numbers.insert({4, 400});
    std_numbers.insert({4, 400});
    numbers[5] = 500;
    std_numbers[5] = 500;
    assert_same_map(numbers, std_numbers);

    numbers.erase_Key(1);
    std_numbers.erase(1);
    numbers.erase_Key(5);
    std_numbers.erase(5);
    assert_same_map(numbers, std_numbers);
    assert_missing_key(numbers, 1);
    assert_missing_key(numbers, 5);

    std::cout << "All UnorderedMAP checks passed." << std::endl;
}
