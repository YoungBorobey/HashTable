#include <iostream>
#include <vector>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <list>

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    std::vector<std::list<typename std::list<std::pair<const KeyType, ValueType>>::iterator>> Data;
    std::list<std::pair<const KeyType, ValueType>> all_elems;
    Hash my_H;
    size_t count_el;

public:
    HashMap(const Hash& a = Hash()) : my_H(a), count_el(0) {
        Data.resize(9973);
    }

    template<typename It>
    HashMap(It start, It end, const Hash& a = Hash()) : my_H(a), count_el(0) {
        Data.resize(9973);
        while (start != end) {
            insert(*start);
        }
    }

    HashMap(const std::initializer_list<std::pair<KeyType, ValueType>>& arr,
    const Hash& a = Hash()) : my_H(a), count_el(0) {
        Data.resize(9973);
        for (const auto& i : arr) {
            insert(i);
        }
    }

    size_t size() const {
        return count_el;
    }

    bool empty() const {
        return count_el == 0;
    }

    Hash hash_function() const {
        return my_H;
    }

    void insert(const std::pair<const KeyType, ValueType>& elem) {
        size_t id = my_H(elem.first) % 9973;
        auto i = Data[id].begin();
        while (i != Data[id].end() && !((*(*i)).first == elem.first)) {
            ++i;
        }
        if (i == Data[id].end()) {
            ++count_el;
            all_elems.push_back(elem);
            auto it = all_elems.end();
            --it;
            Data[id].push_back(it);
        }
    }

    void erase(const KeyType& key) {
        size_t id = my_H(key) % 9973;
        auto i = Data[id].begin();
        while (i != Data[id].end() && !((*(*i)).first == key)) {
            ++i;
        }
        if (i != Data[id].end()) {
            Data[id].erase(i);
            all_elems.erase(*i);
            --count_el;
        }
    }

    typedef typename std::list<std::pair<const KeyType, ValueType>>::iterator iterator;
    typedef typename std::list<std::pair<const KeyType, ValueType>>::const_iterator const_iterator;

    iterator begin() {
        return all_elems.begin();
    }
    iterator end() {
        return all_elems.end();
    }
    const_iterator begin() const {
        return all_elems.cbegin();
    }
    const_iterator end() const {
        return all_elems.cend();
    }
    iterator find(const KeyType& key) {
        size_t id = my_H(key) % 9973;
        auto i = Data[id].begin();
        while (i != Data[id].end() && !((*(*i)).first == key)) {
            ++i;
        }
        if (i == Data[id].end()) {
            return end();
        }
        return *i;
    }

    const_iterator find(const KeyType& key) const {
        size_t id = my_H(key) % 9973;
        auto i = Data[id].cbegin();
        while (i != Data[id].end() && !((*(*i)).first == key)) {
            ++i;
        }
        if (i == Data[id].end()) {
            return end();
        }
        return *i;
    }

    ValueType& operator[](const KeyType& key) {
        size_t id = my_H(key) % 9973;
        auto i = Data[id].begin();
        while (i != Data[id].end() && !((*(*i)).first == key)) {
            ++i;
        }
        if (i == Data[id].end()) {
            all_elems.push_back(std::make_pair(key, ValueType()));
            auto it = all_elems.end();
            --it;
            Data[id].push_back(it);
            ++count_el;
            return (*it).second;
        }
        return (*(*i)).second;
    }

    const ValueType& at(const KeyType& key) const {
        size_t id = my_H(key) % 9973;
        auto i = Data[id].begin();
        while (i != Data[id].end() && !((*(*i)).first == key)) {
            ++i;
        }
        if (i == Data[id].end()) {
            throw std::out_of_range("Out Of Range");
        }
        return (*(*i)).second;
    }

    void clear() {
        if (count_el == 0) {
            return;
        }
        for (int i = 0; i < 9973; ++i) {
            if (Data[i].size() != 0) {
                Data[i].clear();
            }
        }
        count_el = 0;
        all_elems.clear();
    }
};


