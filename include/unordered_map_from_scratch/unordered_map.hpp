#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>

#include "unordered_map_from_scratch/abstract_data.hpp"

template<typename Key, typename Data, typename dobLincList = node<Key, Data>,
        typename Iter = iterator<dobLincList, Key, Data>>
class UnorderedMAP : public abstract_data_t<Key, Data> {
public:
    using key_type = Key;
    using mapped_type = Data;
    using value_type = std::pair<Key, Data>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using hasher = std::hash<Key>;
    using key_equal = std::equal_to<Key>;

    struct node_type {
        // TODO: implement node handle support like std::unordered_map::node_type.
    };

    ~UnorderedMAP() override {
        UnorderedMAP::clear();
        delete[] hash_table;
        hash_table = nullptr;
        size_table = 0;
    };

    explicit UnorderedMAP(size_t size) {
        hash_table = new dobLincList *[size];
        for (size_t i = 0; i < size; ++i) {
            hash_table[i] = nullptr;
        }
        head = last = nullptr;
        size_Bucket = 0;
        size_table = size;
    }

    UnorderedMAP(std::initializer_list<std::pair<Key, Data>> pair_list) {
        UnorderedMAP tmp(23);
        for (const auto &item: pair_list) {
            tmp.emplace(item.first, item.second);
        }
        this->swap(tmp);
    }


    UnorderedMAP() {
        hash_table = new dobLincList *[23]();
        head = last = nullptr;
        size_Bucket = 0;
        size_table = 23;
    }


    explicit UnorderedMAP(const UnorderedMAP<Key, Data> &orig) {
        UnorderedMAP EMP(23);
        if (orig.empty()) {
            this->swap(EMP);
        } else {
            for (auto iter = orig.cbegin(); iter != orig.cend(); ++iter) {
                EMP.emplace(iter->first, iter->second);
            }
            this->swap(EMP);
        }
    }

    UnorderedMAP(const iterator<dobLincList, Key, Data> &Begin, const iterator<dobLincList, Key, Data> &End) {
        UnorderedMAP tmp(23);
        for (auto item = Begin; item != End; ++item) {
            tmp.emplace(item->first, *item);
        }
        this->swap(tmp);
    }

    friend std::ostream &operator<<(std::ostream &output, const UnorderedMAP &map) {
        for (auto print = map.cbegin(); print != map.cend(); ++print)
            output << "[" << print->first << "->" << print->second << "]\n";
        return output;
    }


    // TODO: std::unordered_map has overloads for const key_type&, key_type&&,
    // and transparent lookup. Current version accepts Key by value.
    Data &operator[](Key key_find) override {
        try {
            return (at(key_find));
        }
        catch (const std::out_of_range &) {
            this->emplace(key_find, Data{});

            return at(key_find);
        }
    }

    // TODO: std::unordered_map does not provide const operator[].
    // Keep it for current project compatibility, but remove or redesign later.
    Data &operator[](Key key_find) const override {
        return at(key_find);
    }

    // TODO: return const Data& in the const overload, matching std::unordered_map::at.
    Data &at(Key key_) const override {
        dobLincList *find_ = hash_table[hash_function(key_) % size_table];

        while (find_ != nullptr) {
            if (find_->pair.first == key_) {
                return find_->pair.second;
            }
            find_ = find_->next;
        }

        throw std::out_of_range("Key not found");
    }

    Data &at(Key key_) override {
        dobLincList *find_ = hash_table[hash_function(key_) % size_table];

        while (find_ != nullptr) {
            if (find_->pair.first == key_) {
                return (find_->pair.second);
            }
            find_ = find_->next;
        }

        throw std::out_of_range("Key not found");
    }

    [[nodiscard]] bool is_equal(const UnorderedMAP &obj1) const {
        if (this->size_Bucket != obj1.size_Bucket)return false;
        for (auto it = obj1.cbegin(); it != obj1.cend(); ++it) {
            try {
                if (this->at(it->first) != obj1[it->first])return false;
            }
            catch (const std::out_of_range &) {
                return false;
            }
        }

        return true;
    }

    [[nodiscard]] bool operator==(const UnorderedMAP &orig) const {
        return is_equal(orig);
    }

    bool operator==(const abstract_data_t<Key, Data> &orig) const override {
        if (!dynamic_cast<const UnorderedMAP *>(&orig)) {
            return false;
        }
        return is_equal(dynamic_cast<const UnorderedMAP &>(orig));
    }

    UnorderedMAP &operator=(const UnorderedMAP &copy) {
        if (this == &copy) {
            return *this;
        }

        clear();
        for (auto it = copy.cbegin(); it != copy.cend(); ++it) {
            this->push(it->first, it->second);
        }
        return *this;
    }


    void swap(UnorderedMAP &abstractData) noexcept {
        std::swap(this->hash_table, abstractData.hash_table);
        std::swap(this->head, abstractData.head);
        std::swap(this->last, abstractData.last);
        std::swap(this->size_table, abstractData.size_table);
        std::swap(this->size_Bucket, abstractData.size_Bucket);
    }

    void merge(const abstract_data_t<Key, Data> &obgTMP) override {
        // TODO: std::unordered_map::merge transfers nodes and leaves duplicates in source.
        // Current implementation copies/assigns values and does not modify source.
        for (auto it = obgTMP.cbegin(); it != obgTMP.cend(); ++it) {
            (*this)[it->first] = it->second;
        }
    }

    [[nodiscard]] size_t max_size() const override {
        // TODO: std::unordered_map::max_size returns maximum possible element count,
        // not the current bucket-table size.
        return size_table;
    }

    [[nodiscard]] size_t size() const override {
        return size_Bucket;
    }

    void push(Key key, Data data) override {
        this->emplace(key, data);
    }

    void emplace(Key key, Data value) override {
        // TODO: std::unordered_map::emplace returns pair<iterator, bool> and accepts variadic args.
        if ((size_Bucket * 100) / size_table >= 100) {
            reHashing(size_table * 2);
        }

        if (head == nullptr) {
            head = new dobLincList(key, value);
            head->pair.first = key;
            head->pair.second = value;
            head->hash = (int) hash_function(key);
            hash_table[head->hash % size_table] = head;
            last = head;
            size_Bucket++;
        } else {
            auto *tmp = new dobLincList(key, value);
            tmp->hash = hash_function(key);
            tmp->pair.first = key;
            tmp->pair.second = value;
            if (hash_table[tmp->hash % size_table] == nullptr) {
                hash_table[tmp->hash % size_table] = tmp;
                last->next = tmp;
                tmp->prev = last;
                last = tmp;
                size_Bucket++;
            } else {
                if(dobLincList::insertNode(hash_table[(tmp->hash) % size_table],
                                           tmp, size_table)){
                    if (tmp->next == nullptr) last = tmp;
                    size_Bucket++;
                } else {
                    delete tmp;
                }

            }
        }
    }

    void try_emplace(Key key, Data value) override {
        // TODO: std::unordered_map::try_emplace returns pair<iterator, bool>,
        // constructs mapped value only when insertion happens, and supports hint overloads.
        if ((size_Bucket * 100) / size_table >= 100) {
            reHashing(size_table * 2);
        }

        if (head == nullptr) {
            head = new dobLincList(key, value);
            head->pair.first = key;
            head->pair.second = value;
            head->hash = (int) hash_function(key);
            hash_table[head->hash % size_table] = head;
            last = head;
            size_Bucket++;
        } else {
            auto *tmp = new dobLincList(key, value);
            tmp->hash = hash_function(key);
            tmp->pair.first = key;
            tmp->pair.second = value;
            if (hash_table[tmp->hash % size_table] == nullptr) {
                hash_table[tmp->hash % size_table] = tmp;
                last->next = tmp;
                tmp->prev = last;
                last = tmp;
                size_Bucket++;
            } else {

                   if(dobLincList::insertNode(hash_table[(tmp->hash) % size_table],
                                            tmp, size_table)){
                       if (tmp->next == nullptr) last = tmp;
                       size_Bucket++;
                   } else {
                       delete tmp;
                   }


            }
        }
    }

    void insert_or_assign(Key key, Data value) override {
        // TODO: std::unordered_map::insert_or_assign returns pair<iterator, bool>
        // or iterator for hint overloads. Current version returns void.
        if ((size_Bucket * 100) / size_table >= 100) {
            reHashing(size_table * 2);
        }

        if (head == nullptr) {
            head = new dobLincList(key, value);
            head->pair.first = key;
            head->pair.second = value;
            head->hash = (int) hash_function(key);
            hash_table[head->hash % size_table] = head;
            last = head;
            size_Bucket++;
        } else {
            auto *tmp = new dobLincList(key, value);
            tmp->hash = hash_function(key);
            tmp->pair.first = key;
            tmp->pair.second = value;
            if (hash_table[tmp->hash % size_table] == nullptr) {
                hash_table[tmp->hash % size_table] = tmp;
                last->next = tmp;
                tmp->prev = last;
                last = tmp;
                size_Bucket++;
            } else {
                if (dobLincList::insertNodeEqKey(hash_table[(tmp->hash) % size_table],
                                                 tmp, size_table)) {
                    if (tmp->next == nullptr) last = tmp;
                    size_Bucket++;
                } else {
                    delete tmp;
                }
            }
        }
    }

    static void insert_or_assign(Key key, Data value, UnorderedMAP &obj) {
        obj.insert_or_assign(key, value);

    }


    void printList(const UnorderedMAP<Key, Data> &a) {
        for (int i = 0; i < a.size_table; ++i) {
            dobLincList *tmp = a.hash_table[hash_function(i) % a.size_table];
            while (tmp) {
                if (tmp->key == i) {
                    std::cout << tmp->data << " ";
                    break;
                }
                tmp = tmp->next;
            }
        }
    }

    [[nodiscard]] bool empty() const override {
        if (head == nullptr && last == nullptr
            && size_Bucket == 0)
            return true;
        return false;
    }

    void reHashing(size_t size) override {
        // TODO: rename to rehash(size_t). Standard rehash uses at least n buckets
        // and respects max_load_factor.
        if (head == nullptr) {
            UnorderedMAP TMP(size);
            swap(TMP);
            return;
        } else {
            UnorderedMAP new_map(size);
            for (auto it = begin(); it != end(); ++it) {
                new_map.emplace(it->first, it->second);
            }
            swap(new_map);
        }
    }


    void clear() override {
        dobLincList *current = head;
        while (current) {
            dobLincList *temp = current;
            current = current->next;

            delete temp;
        }
        for (size_t i = 0; hash_table != nullptr && i < size_table; ++i) {
            hash_table[i] = nullptr;
        }
        head = nullptr;
        last = nullptr;
        size_Bucket = 0;
    }


    Iter begin() override {
        iterator<dobLincList, Key, Data> a(head);
        return a;
    }

    // TODO: add const overload begin() const. Current const access uses c_begin().
    Iter end() override {
        iterator<dobLincList, Key, Data> a(nullptr);
        return a;
    }

    Iter cbegin() const override {
        iterator<dobLincList, Key, Data> a(head);
        return a;
    }

    Iter cend() const override {
        iterator<dobLincList, Key, Data> a(nullptr);
        return a;
    }


    dobLincList *find_Node(Key &key) override {
        if (hash_table == nullptr || size_table == 0) {
            return nullptr;
        }

        size_t hash_value = hash_function(key);
        dobLincList *current = hash_table[hash_value % size_table];

        while (current) {
            if (current->pair.first == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void erase_Key(Key key) override {
        dobLincList *del = find_Node(key);
        if (del == nullptr)return;

        if (del->prev != nullptr) {
            del->prev->next = del->next;
        } else {
            head = del->next;
        }

        if (del->next != nullptr) {
            del->next->prev = del->prev;
        } else {
            last = del->prev;
        }

        del->clear_node();
        delete del;
        size_Bucket--;

        for (size_t i = 0; i < size_table; ++i) {
            hash_table[i] = nullptr;
        }
        for (dobLincList *current = head; current != nullptr; current = current->next) {
            size_t bucket = current->hash % size_table;
            if (hash_table[bucket] == nullptr) {
                hash_table[bucket] = current;
            }
        }
    }

    Data &front() override {
        if (head == nullptr)throw std::out_of_range("no knots");
        return head->pair.second;
    }

    Data &back() override {
        if (last == nullptr)throw std::out_of_range("no knots");
        return last->pair.second;
    }


    iterator<dobLincList, Key, Data>
    insert(iterator<dobLincList, Key, Data> beg, std::pair<Key, Data> ins) override {
        // TODO: std::unordered_map uses const_iterator hint and returns iterator.
        // Current hint is not used to optimize insertion.
        if (beg == end()) {
            emplace(ins.first, ins.second);
            return find(ins.first);
        }
        /*bool belonging = true;
        for (auto it : *this)
        {
            if (beg == it)belonging = false;
        }
        if (belonging)return beg;*/
        emplace(ins.first, ins.second);
        return beg;
    };

    void insert(std::pair<Key, Data> ins) override {
        // TODO: std::unordered_map::insert returns pair<iterator, bool>.

        emplace(ins.first, ins.second);
    };

    void reserve(size_t new_size) override {
        // TODO: std::unordered_map::reserve(n) prepares for n elements.
        // Current implementation treats n as bucket count.
        reHashing(new_size);
    }

    Iter find(const Key &key) override {
        Key tmp = key;
        dobLincList *del = find_Node(tmp);
        iterator<dobLincList, Key, Data> it(del);
        return it;
    };

    Iter erase(iterator<dobLincList, Key, Data> iter) override {
        // TODO: std::unordered_map has erase(iterator), erase(const_iterator),
        // erase(key), and erase(first, last). This project only partially covers them.
        if (iter == end()) {
            return iter;

        }
        bool belonging = true;
        for (auto &&it: *this) {
            if (*iter == it)belonging = false;
        }
        if (belonging)return iter;
        Iter tmp = iter;
        ++tmp;
        erase_Key(iter->first);
        return tmp;
    };


    void insert_range(std::initializer_list<std::pair<Key, Data>> &pair_list) override {
        // TODO: std::unordered_map::insert_range accepts a compatible range,
        // not only initializer_list by non-const reference.
        for (const auto &item: pair_list) {
            emplace(item.first, item.second);
        }
    };

    static Data erase(abstract_data_t<Key, Data> &map, const Key &key) {
        Data tmp = map.find_Node(key)->pair.second;
        map.erase(map.find_Node(key));
        return tmp;
    };

    [[nodiscard]] float load_factor() const override {
        return static_cast<float>(static_cast<float>(size_Bucket) / static_cast<float>(size_table));
    };

    [[nodiscard]] float max_load_factor() const override {
        return static_cast<float>(max_l_factor);
    };

    // TODO: implement allocator support. Standard unordered_map returns allocator_type.
    static void get_allocator() {
    }

    // TODO: implement move constructor with correct ownership transfer.
    // Current class owns raw pointers, so default move would be unsafe.

    // TODO: implement initializer_list assignment like std::unordered_map::operator=(initializer_list).
    UnorderedMAP &assign_from_initializer_list(std::initializer_list<std::pair<Key, Data>> values) {
        clear();
        for (const auto &item: values) {
            emplace(item.first, item.second);
        }
        return *this;
    }

    // TODO: implement std::unordered_map::emplace_hint semantics.
    Iter emplace_hint(Iter hint, Key key, Data value) {
        emplace(key, value);
        return find(key);
    }

    // TODO: implement insert(first, last) range overload.
    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last_iterator) {
        for (auto it = first; it != last_iterator; ++it) {
            insert(*it);
        }
    }

    // TODO: implement initializer_list insert overload with standard return/behavior details.
    void insert(std::initializer_list<std::pair<Key, Data>> values) {
        for (const auto &item: values) {
            insert(item);
        }
    }

    // TODO: implement extract(iterator). Standard method removes a node and returns node_type.
    node_type extract(Iter position) {
        if (position != end()) {
            erase(position);
        }
        return {};
    }

    // TODO: implement extract(key). Standard method removes the matching node and returns node_type.
    node_type extract(const Key &key) {
        erase_Key(key);
        return {};
    }

    // TODO: implement insert(node_type&&). Standard method reinserts an extracted node.
    std::pair<Iter, bool> insert(node_type &&handle) {
        (void) handle;
        return {end(), false};
    }

    // TODO: implement insert(hint, node_type&&). Standard method reinserts an extracted node using a hint.
    static Iter insert(Iter hint, node_type &&handle) {
        (void) handle;
        return hint;
    }

    // TODO: this is the std::unordered_map erase(key) candidate.
    // It is named erase_by_key for now to avoid clashing with the current abstract erase(Iter).
    size_t erase_by_key(const Key &key) {
        bool existed = contains(key);
        erase_Key(key);
        return existed ? 1 : 0;
    }

    // TODO: implement erase(first, last). Current iterator type is fragile for range erase.
    Iter erase(Iter first, Iter last_iter) {
        while (first != last_iter) {
            first = erase(first);
        }
        return last_iter;
    }

    // TODO: implement standard observers using stored hasher object if custom hashers are added.
    static hasher hash_function() {
        return hasher{};
    }

    // TODO: implement standard key equality observer using stored predicate if custom predicates are added.
    static key_equal key_eq() {
        return key_equal{};
    }

    // TODO: add const find overload returning const_iterator when iterator model is split.
    Iter find(const Key &key) const {
        dobLincList *del = find_node_const(key);
        iterator<dobLincList, Key, Data> it(del);
        return it;
    }

    [[nodiscard]] size_t count(const Key &key) const {
        return contains(key) ? 1 : 0;
    }

    [[nodiscard]] bool contains(const Key &key) const {
        return find_node_const(key) != nullptr;
    }

    // TODO: implement equal_range with proper iterator pair semantics.
    std::pair<Iter, Iter> equal_range(const Key &key) {
        Iter it = find(key);
        if (it == end()) {
            return {end(), end()};
        }

        Iter next = it;
        ++next;
        return {it, next};
    }

    // TODO: implement const equal_range after introducing a real const_iterator.
    std::pair<Iter, Iter> equal_range(const Key &key) const {
        Iter it = find(key);
        if (it == cend()) {
            return {cend(), cend()};
        }

        Iter next = it;
        ++next;
        return {it, next};
    }

    [[nodiscard]] size_t bucket_count() const {
        return size_table;
    }

    // TODO: std::unordered_map::max_bucket_count returns maximum possible bucket count.
    [[nodiscard]] size_t max_bucket_count() const {
        return size_table;
    }

    [[nodiscard]] size_t bucket_size(size_t n) const {
        if (n >= size_table) {
            return 0;
        }

        size_t result = 0;
        for (dobLincList *current = head; current != nullptr; current = current->next) {
            if (current->hash % size_table == n) {
                ++result;
            }
        }
        return result;
    }

    [[nodiscard]] size_t bucket(const Key &key) const {
        if (size_table == 0) {
            return 0;
        }
        return hash_function(key) % size_table;
    }

    // TODO: implement real local_iterator. Current iterator walks the global linked list.
    Iter begin(size_t n) {
        return local_bucket_begin(n);
    }

    // TODO: implement real const local iterator.
    Iter begin(size_t n) const {
        return local_bucket_begin(n);
    }

    // TODO: implement real local bucket end sentinel.
    Iter end(size_t n) {
        (void) n;
        return end();
    }

    // TODO: implement real const local bucket end sentinel.
    Iter end(size_t n) const {
        (void) n;
        return cend();
    }

    // TODO: implement real const local iterator.
    Iter cbegin(size_t n) const {
        return local_bucket_begin(n);
    }

    // TODO: implement real const local bucket end sentinel.
    Iter cend(size_t n) const {
        (void) n;
        return cend();
    }

    // TODO: std::unordered_map exposes max_load_factor(float) setter.
    void max_load_factor(float z) {
        max_l_factor = z;
    }

    // TODO: standard spelling. Keep reHashing for backward compatibility.
    void rehash(size_t n) {
        reHashing(n);
    }

private:
    dobLincList **hash_table{nullptr};
    size_t size_table{23};
    size_t size_Bucket{0};
    dobLincList *head{nullptr};
    dobLincList *last{nullptr};
    float max_l_factor{1};


    [[nodiscard]] int hash_function(Key key) const {
        std::hash<Key> hasher;
        int key_ = abs(static_cast<int>(hasher(key)));

        if (key_ % 2 == 0) {
            double sqrt_result = sqrt(std::abs(key_)) + sqrt(std::abs(key_ + 31));
            return static_cast<int>(sqrt_result * 100000) % 100000;
        } else {
            int result = key_;
            result = (result << 12) | (result >> (sizeof(int) * 8 - 1));
            result ^= ~0x55555555;


            result = (result >= 0) ? result : -result;

            if (result == 0) {
                return 1;
            }

            return result;
        }
    }

    dobLincList *find_node_const(const Key &key) const {
        if (hash_table == nullptr || size_table == 0) {
            return nullptr;
        }

        size_t hash_value = hash_function(key);
        dobLincList *current = hash_table[hash_value % size_table];

        while (current) {
            if (current->pair.first == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    Iter local_bucket_begin(size_t n) const {
        if (hash_table == nullptr || n >= size_table) {
            return Iter(nullptr);
        }
        return Iter(hash_table[n]);
    }
};
