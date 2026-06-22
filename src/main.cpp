#include <iostream>
#include <string>
#include <unordered_map>

#include "unordered_map_from_scratch/unordered_map.hpp"

int main(int argc, char const *argv[]) {


    UnorderedMAP<std::string, std::string> u = {
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"}};

    for (const auto &n: u)
        std::cout << n.first << ": " << n.second << " ";
    std::cout << std::endl;

    u["BLACK"] = "#000000";
    u["WHITE"] = "#FFFFFF";

    for (const auto &n: u)
        std::cout << n.first << ": " << n.second << " ";

    UnorderedMAP<std::string, std::string> uno = {
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"}};

    std::unordered_map<std::string, std::string> map = {
            {"RED",   "#FF0000"},
            {"GREEN", "#00FF00"},
            {"BLUE",  "#0000FF"}};
    std::cout << std::endl;
    std::cout << std::endl;

    for (const auto &n: uno)
        std::cout << n.first << ": " << n.second << " ";
    std::cout << std::endl;

    for (const auto &n: map)
        std::cout << n.first << ": " << n.second << " ";

    map.reserve(100);
    uno.reserve(100);

    std::cout << std::endl;
    std::cout << std::endl;

    for (const auto &n: uno)
        std::cout << n.first << ": " << n.second << " ";
    std::cout << std::endl;

    for (const auto &n: map)
        std::cout << n.first << ": " << n.second << " ";


    std::cout << map.load_factor() << "\n";
    std::cout << uno.load_factor() << "\n";
    std::cout << map.max_load_factor() << "\n";
    std::cout << map.max_load_factor() << "\n\n";

    std::unordered_map<std::string, std::string> map_2 = {
            {"tt",   "tttttt"},
            {"ttt",  "tttttttttttt"},
            {"tttt", "tttttttttttt"}};

    UnorderedMAP<std::string, std::string> uno_2 = {
            {"tt",   "tttttt"},
            {"ttt",  "tttttttttttt"},
            {"tttt", "tttttttttttt"}};

    map.merge(map_2);
    uno.merge(uno_2);
    std::cout << std::endl;
    std::cout << std::endl;

    for (const auto &n: uno)
        std::cout << n.first << ": " << n.second << " ";
    std::cout << std::endl;

    for (const auto &n: map)
        std::cout << n.first << ": " << n.second << " ";


    std::cout << map.load_factor() << "\n";
    std::cout << uno.load_factor() << "\n";
    std::cout << map.max_load_factor() << "\n";
    std::cout << map.max_load_factor() << "\n\n";




    std::cout << std::endl;
    std::cout << std::endl;

    for (const auto &n: uno)
        std::cout << n.first << ": " << n.second << " ";
    std::cout << std::endl;

    for (const auto &n: map)
        std::cout << n.first << ": " << n.second << " ";



    std::unordered_map<int, int> st = {
            {1,   100},
            {2,  200},
            {3, 300}};

    UnorderedMAP<int, int> my = {
            {1,   100},
            {2,  200},
            {3, 300}};


    st.emplace(1,200);
    my.emplace(1,200);

    st.try_emplace(3,200);
    my.try_emplace(3,200);
    st.try_emplace(2,100);
    my.try_emplace(2,100);

    st.insert_or_assign(3,200);
    my.insert_or_assign(3,200);
    st.insert_or_assign(2,100);
    my.insert_or_assign(2,100);



    st.insert({30,200});
    my.insert({30,200});
    st.insert({20,100});
    my.insert({20,100});

    st.insert({30,200});
    my.insert({30,200});
    st.insert({20,100});
    my.insert({20,100});


    st.insert(st.begin(),{33,200});
    my.insert(my.begin(),{33,200});
    st.insert(st.begin(),{22,100});
    my.insert(my.begin(),{22,100});


    std::cout << std::endl;
    std::cout << std::endl;

    for (const auto &n: st)
        std::cout << n.first << ": " << n.second << " ";
    std::cout << std::endl;

    for (const auto &n: my)
        std::cout << n.first << ": " << n.second << " ";



}
