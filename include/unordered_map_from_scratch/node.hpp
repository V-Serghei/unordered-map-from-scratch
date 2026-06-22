#pragma once

#include <cstddef>
#include <utility>


// остальные необходимые библиотеки здесь
template<typename Key, typename Data>
class node {
public:
    node *next{nullptr};
    node *prev{nullptr};
    std::pair<Key, Data> pair;
    int hash{0};

    node() = default;

    ~node() = default;

    node(Key key1, Data data1) {
        hash = 0;
        pair.first = key1;
        pair.second = data1;
        next = nullptr;
        prev = nullptr;
    }


    node(Key key1, Data data1, int hash1) {
        hash = hash1;
        pair.first = key1;
        pair.second = data1;
        next = nullptr;
        prev = nullptr;
    }

    static bool insertNode(node *&list, node *&current, size_t size) {
        node *now = list;
        while (now->next != nullptr) {
            if (now->pair.first == current->pair.first) {
                return false;
            }
            if (now->next->hash % size != current->hash % size) {
                node *tmp = now->next;
                now->next = current;
                current->next = tmp;
                tmp->prev = current;
                current->prev = now;
                return true;
            }
            now = now->next;
        }
        if (now->pair.first == current->pair.first) {
            return false;
        }
        now->next = current;
        current->prev = now;
        return true;
    }

    static bool insertNodeEqKey(node *&list, node *&current, size_t size) {
        node *now = list;
        while (now->next != nullptr) {
            if (now->pair.first == current->pair.first) {
                now->pair.second = current->pair.second;
                return false;
            }
            if (now->next->hash % size != current->hash % size) {
                node *tmp = now->next;
                now->next = current;
                current->next = tmp;
                tmp->prev = current;
                current->prev = now;
                return true;
            }
            now = now->next;
        }
        if (now->pair.first == current->pair.first) {
            now->pair.second = current->pair.second;
            return false;
        }
        now->next = current;
        current->prev = now;
        return true;
    }

    node clear_node() {
        next = nullptr;
        hash = 0;
        prev = nullptr;
        return *this;
    }
};
