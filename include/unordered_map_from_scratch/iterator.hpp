#pragma once

#include <cstddef>
#include <iterator>
#include <utility>



template<typename dobLincList, typename Key, typename Data>
class iterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::pair<Key, Data>;
    using pointer = value_type *;
    using reference = value_type &;
    using difference_type = std::ptrdiff_t;

    ~iterator() = default;

    iterator() = default;

    iterator(const iterator &It) {
        this->p = It.p;
    };

    explicit iterator(dobLincList *node) {
        this->p = node;
    };

    iterator &operator++() {
        p = p->next;
        return *this;
    };

    const Data &operator*() const {
        return p->pair;
    }

    pointer operator->() const {
        return &(p->pair);
    }

    iterator operator++(int) {
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    iterator &operator--() {
        p = p->prev;
        return *this;
    }

    iterator operator--(int) {
        iterator *tmp = this;
        p = p->prev;
        return *tmp;
    }

    const value_type &operator*() {
        return p->pair;
    }

    pointer operator->() {
        return &(p->pair);
    }

    friend bool operator==(const iterator &iterator1, const iterator &iterator2) {
        return iterator1.p == iterator2.p;
    }

    friend bool operator!=(const iterator &iterator1, const iterator &iterator2) {
        return !(iterator1 == iterator2);
    }

private:
    dobLincList *p{nullptr};
};
