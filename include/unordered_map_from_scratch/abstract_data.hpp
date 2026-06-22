#pragma once

#include <initializer_list>
#include <utility>

#include "unordered_map_from_scratch/iterator.hpp"
#include "unordered_map_from_scratch/node.hpp"



template<typename Key, typename Data, typename dobLincList = node<Key, Data>, typename Iter = iterator<
        dobLincList, Key, Data>>
class abstract_data_t {
public:
    // методы из предыдущей лабораторной работы здесь
    virtual ~abstract_data_t() = default;

    virtual bool operator==(const abstract_data_t &) const = 0;

    abstract_data_t &operator= (const abstract_data_t &) = default;

    virtual Data &operator[](Key) = 0;

    virtual Data &operator[](Key key_find) const = 0;

    [[nodiscard]] virtual Data &at(Key) = 0;

    [[nodiscard]] virtual Data &at(Key) const = 0;

    [[nodiscard]] virtual size_t size() const = 0;

    virtual void push(Key, Data) = 0;

    virtual void emplace(Key, Data) = 0;

    virtual void try_emplace(Key key, Data value) = 0;

    virtual void insert_or_assign(Key key, Data value) = 0;

    [[nodiscard]] virtual bool empty() const = 0;

    virtual void clear() = 0;

    virtual void reHashing(size_t) = 0;

    virtual void merge(const abstract_data_t<Key, Data> &) = 0;

    [[nodiscard]] virtual size_t max_size() const = 0;

    virtual Iter begin() = 0;

    virtual Iter end() = 0;

    virtual Iter cbegin() const = 0;

    virtual Iter cend() const = 0;

    virtual Data &back() = 0;

    virtual Data &front() = 0;

    virtual dobLincList *find_Node(Key &key) = 0;

    virtual void erase_Key(Key key) = 0;


    // рекомендуемые прототипы функций:
    virtual Iter find(const Key &) = 0;

    virtual void reserve(size_t new_size) = 0;

    virtual void insert(std::pair<Key, Data> ins) = 0;

    virtual Iter insert(Iter beg, std::pair<Key, Data> ins) = 0;

    virtual Iter erase(Iter) = 0;

    [[nodiscard]]virtual float load_factor() const = 0;

    [[nodiscard]]virtual float max_load_factor() const = 0;

    virtual void insert_range(std::initializer_list<std::pair<Key, Data>> &) = 0;
};
