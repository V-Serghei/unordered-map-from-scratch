#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "unordered_map_from_scratch/abstract_data.hpp"

template<typename Key, typename Data, typename dobLincList = node<Key, Data>,
        typename Iter = iterator<dobLincList, Key, Data>>
class UnorderedMAP : public abstract_data_t<Key, Data> {
public:
    ~UnorderedMAP() override {
        clear();
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
            for (auto iter = orig.cbegin(); iter != orig.cend(); iter++) {
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
        for (auto print = map.cbegin(); print != map.cend(); print++)
            output << "[" << print->first << "->" << print->second << "]" << std::endl;
        return output;
    }


    Data &operator[](Key key_find) override {
        try {
            return (at(key_find));
        }
        catch (const std::out_of_range &e) {
            // std::cerr << "Exception ->  " << e.what() << std::endl;
            this->emplace(key_find, Data{});

            return at(key_find);
        }
    }

    Data &operator[](Key key_find) const override {
        return at(key_find);
    }

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
        for (auto it = obj1.cbegin(); it != obj1.cend(); it++) {
            try {
                if (this->at(it->first) != obj1[it->first])return false;
            }
            catch (const std::out_of_range &) {
                return false;
            }
        }

        return true;
    }

    bool operator==(const abstract_data_t<Key, Data> &orig) const override {
        if (!dynamic_cast<const UnorderedMAP *>(&orig)) {
            return false;
        }
        return is_equal(dynamic_cast<const UnorderedMAP &>(orig));
    }

    bool operator==(const UnorderedMAP &orig) const {
        return is_equal(orig);
    }

    UnorderedMAP &operator=(const abstract_data_t<Key, Data> &orig) override {
        if (this == &orig) {
            return *this;
        }

        const auto &copy = dynamic_cast<const UnorderedMAP &>(orig);
        return *this = copy;
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


    void swap(UnorderedMAP &abstractData) {
        std::swap(this->hash_table, abstractData.hash_table);
        std::swap(this->head, abstractData.head);
        std::swap(this->last, abstractData.last);
        std::swap(this->size_table, abstractData.size_table);
        std::swap(this->size_Bucket, abstractData.size_Bucket);
    }

    void merge(const abstract_data_t<Key, Data> &obgTMP) override {
        for (auto it = obgTMP.cbegin(); it != obgTMP.cend(); it++) {
            (*this)[it->first] = it->second;
        }
    }

    size_t max_size() const override {
        return size_table;
    }

    size_t size() const override {
        return size_Bucket;
    }

    void push(Key key, Data data) override {
        this->emplace(key, data);
    }

    void emplace(Key key, Data value) override {
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

    bool empty() const override {
        if (head == nullptr && last == nullptr
            && size_Bucket == 0)
            return true;
        return false;
    }

    void reHashing(size_t size) override {
        if (head == nullptr) {
            UnorderedMAP TMP(size);
            swap(TMP);
            return;
        } else {
            UnorderedMAP new_map(size);
            for (auto it = begin(); it != end(); it++) {
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

    Iter end() override {
        iterator<dobLincList, Key, Data> a(nullptr);
        return a;
    }

    const Iter cbegin() const override {
        const iterator<dobLincList, Key, Data> a(head);
        return a;
    }

    const Iter cend() const override {
        const iterator<dobLincList, Key, Data> a(nullptr);
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
    insert(iterator<dobLincList, Key, Data> beg, const std::pair<Key, Data> ins) override {
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

    void insert(const std::pair<Key, Data> ins) override {

        emplace(ins.first, ins.second);
    };

    void reserve(size_t new_size) override {
        reHashing(new_size);
    }

    Iter find(const Key &key) override {
        Key tmp = key;
        dobLincList *del = find_Node(tmp);
        iterator<dobLincList, Key, Data> it(del);
        return it;
    };

    Iter erase(iterator<dobLincList, Key, Data> iter) override {
        if (iter == end()) {
            return iter;

        }
        bool belonging = true;
        for (auto &&it: *this) {
            if (*iter == it)belonging = false;
        }
        if (belonging)return iter;
        Iter tmp = iter;
        tmp++;
        erase_Key(iter->first);
        return tmp;
    };


    void insert_range(std::initializer_list<std::pair<Key, Data>> &pair_list) override {
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
            result ^= 0xAAAAAAAA;


            result = (result >= 0) ? result : -result;

            if (result == 0) {
                return 1;
            }

            return result;
        }
    }
};
