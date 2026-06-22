#include <cassert>
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>


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


template<typename Key, typename Data, typename dobLincList = node<Key, Data>, typename Iter = iterator<
        dobLincList, Key, Data>>
class abstract_data_t {
public:
    // методы из предыдущей лабораторной работы здесь
    virtual ~abstract_data_t() = default;

    virtual bool operator==(const abstract_data_t &) const = 0;

    virtual abstract_data_t &operator= (const abstract_data_t &) = 0;

    virtual Data &operator[](Key) = 0;

    virtual Data &operator[](Key key_find) const = 0;

    [[nodiscard]] virtual Data &at(Key) = 0;

    [[nodiscard]] virtual Data &at(Key) const = 0;

    [[nodiscard]] virtual size_t size() const = 0;

    virtual void push(Key, Data) = 0;

    virtual void emplace(Key, Data) = 0;

    virtual void try_emplace(Key key, Data value) = 0;

    virtual void insert_or_assign(Key key, Data value) = 0;

    virtual bool empty() const = 0;

    virtual void clear() = 0;

    virtual void reHashing(size_t) = 0;

    virtual void merge(const abstract_data_t<Key, Data> &) = 0;

    [[nodiscard]] virtual size_t max_size() const = 0;

    virtual Iter begin() = 0;

    virtual Iter end() = 0;

    virtual const Iter cbegin() const = 0;

    virtual const Iter cend() const = 0;

    virtual Data &back() = 0;

    virtual Data &front() = 0;

    virtual dobLincList *find_Node(Key &key) = 0;

    virtual void erase_Key(Key key) = 0;


    // рекомендуемые прототипы функций:
    virtual Iter find(const Key &) = 0;

    virtual void reserve(size_t new_size) = 0;

    virtual void insert(const std::pair<Key, Data> ins) = 0;

    virtual Iter insert(Iter beg, const std::pair<Key, Data> ins) = 0;

    virtual Iter erase(Iter) = 0;

    [[nodiscard]]virtual float load_factor() const = 0;

    [[nodiscard]]virtual float max_load_factor() const = 0;

    virtual void insert_range(std::initializer_list<std::pair<Key, Data>> &) = 0;
};


template<typename Key, typename Data, typename dobLincList = node<Key, Data>,
        typename Iter = iterator<dobLincList, Key, Data>>
class UnorderedMAP : public abstract_data_t<Key, Data> {
public:
    ~UnorderedMAP() override {
        clear();
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
            if (((*this)[it->first]) == (obj1[it->first]))return false;
        }

        return true;
    }

    bool operator==(const abstract_data_t<Key, Data> &orig) const override {
        if (!dynamic_cast<const UnorderedMAP *>(&orig)) {
            return false;
        }
        return is_equal(dynamic_cast<const UnorderedMAP &>(orig));
    }

    UnorderedMAP &operator=(const abstract_data_t<Key, Data> &orig) override {
        abstract_data_t<Key, Data> *copy = new UnorderedMAP(dynamic_cast<const UnorderedMAP &>(orig));
        for (auto it = copy->begin(); it != copy->end(); ++it) {
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
        if (head == nullptr) {
            if (hash_table != nullptr) {
                delete[] hash_table;
                hash_table = nullptr;
            }
            return;
        }
        dobLincList *current = head;
        while (current) {
            dobLincList *temp = current;
            current = current->next;

            delete temp;
        }
        delete[]hash_table;
        hash_table = nullptr;
        head = nullptr;
        last = nullptr;
        size_table = 0;
        size_Bucket = 0;
    }


    Iter begin() override {
        iterator<dobLincList, Key, Data> a(head);
        return a;
    }

    Iter end() override {
        iterator<dobLincList, Key, Data> a(last->next);
        return a;
    }

    const Iter cbegin() const override {
        const iterator<dobLincList, Key, Data> a(head);
        return a;
    }

    const Iter cend() const override {
        const iterator<dobLincList, Key, Data> a(last->next);
        return a;
    }


    dobLincList *find_Node(Key &key) override {
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
        if (head->next == nullptr) {
            clear();
            return;
        }
        if (hash_table[hash_function(del->pair.first) % size_table]->next == nullptr) {
            dobLincList *prev_ = del->prev;
            prev_->next = del->next;
            del->clear_node();
            size_Bucket--;
            return;
        }
        if (((hash_table[hash_function(del->pair.first) % size_table]->next->hash) % size_table)
            != (del->hash % size_table))
            hash_table[hash_function(del->pair.first) % size_table] = nullptr;
        dobLincList *prev_ = del->prev;
        if (prev_ == nullptr)head = del->next;
        else prev_->next = del->next;
        del->clear_node();
        size_Bucket--;
    }

    Data &front() override {
        if (head == nullptr)throw std::out_of_range("no knots");
        return head->pair.second;
    }

    Data &back() override {
        return last->pair.second;
    }


    iterator<dobLincList, Key, Data>
    insert(iterator<dobLincList, Key, Data> beg, const std::pair<Key, Data> ins) override {
        if (beg == end())emplace(ins.first, ins.second);
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
            erase_Key(last->pair.first);
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
