#include <iostream>
#include <vector>
#include <initializer_list>
#include <utility>
#include <stdexcept>
#include <list>
template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    std::vector<std::vector<typename std::list<std::pair<const KeyType,
    ValueType>>::iterator>> Data; // data[pos].size() == 2 means marked position -- elem was deleted. When next relocate will happen, this node will be deleted from data
    std::list<std::pair<const KeyType, ValueType>> all_elems;
    Hash my_H;
    size_t count_el;
    size_t cap;

public:
    HashMap(const Hash& a = Hash()) : my_H(a), count_el(0) {
        Data.resize(2);
        cap = 2;
    }

    template<typename It>
    HashMap(It start, It end, const Hash& a = Hash()) : my_H(a), count_el(0) {
        Data.resize(2);
        cap = 2;
        while (start != end) {
            insert(*start);
        }
    }

    HashMap(const std::initializer_list<std::pair<KeyType, ValueType>>& arr,
    const Hash& a = Hash()) : my_H(a), count_el(0) {
        cap = 2;
        Data.resize(2);
        for (const auto& i : arr) {
            insert(i);
        }
    }

    void relocate() { // do relocation when the table is too big or too small compared to count_el
        if (count_el >= cap / 4 && count_el < cap / 2) {
            return;
        }
        size_t new_size = 0;
        if (count_el < cap / 4) {
            new_size = cap / 2;
        } else if (count_el >= cap / 2) {
            new_size = cap * 2;
        }
        if (new_size == 0) {
            return;
        }
        cap = new_size;
        for (int i = 0; i < Data.size(); ++i) {
            Data[i].clear();
        }
        Data.clear();
        Data.resize(new_size);
        for (auto i = all_elems.begin(); i != all_elems.end(); ++i) {
            size_t id = my_H((*i).first) % cap;
            while (Data[id].size() != 0) {
                ++id;
                id %= cap;
            }
            Data[id].push_back(i);
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
        size_t id = my_H(elem.first) % cap;
        while (Data[id].size() == 1) {
            if ((*(*Data[id].begin())).first == elem.first) {
                break;
            }
            id += 1;
            id %= cap;
        }
        if (Data[id].size() == 0) {
            all_elems.push_back(elem);
            auto it = all_elems.end();
            --it;
            Data[id].push_back(it);
            ++count_el;
            relocate();
        } else if (Data[id].size() == 2) {
            all_elems.push_back(elem);
            auto it = all_elems.end();
            --it;
            Data[id].clear();
            Data[id].push_back(it);
            ++count_el;
            relocate();
        }
    }

    void erase(const KeyType& key) {
        size_t id = my_H(key) % cap;
        size_t pos = 0;
        while (Data[id].size() != 0) {
            if (Data[id].size() != 2 && (*Data[id][0]).first == key) {
                break;
            }
            ++id;
            id %= cap;
        }
        if (Data[id].size() != 0) {
            --count_el;
            all_elems.erase(Data[id][0]);
            Data[id].push_back(Data[id][0]);
            relocate();
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
        size_t id = my_H(key) % cap;
        size_t pos = 0;
        while (Data[id].size() != 0) {
            if (Data[id].size() != 2 && (*Data[id][0]).first == key) {
                break;
            }
            ++id;
            id %= cap;
        }
        if (Data[id].size() == 0) {
            return end();
        }
        return (*Data[id].begin());
    }

    const_iterator find(const KeyType& key) const {
        size_t id = my_H(key) % cap;
        size_t pos = 0;
        while (Data[id].size() != 0) {
            if (Data[id].size() != 2 && (*Data[id][0]).first == key) {
                break;
            }
            ++id;
            id %= cap;
        }
        if (Data[id].size() == 0) {
            return end();
        }
        return (*Data[id].begin());
    }

    ValueType& operator[](const KeyType& key) {
        size_t id = my_H(key) % cap;
        while (Data[id].size() != 0) {
            if (Data[id].size() != 2 && (*Data[id][0]).first == key) {
                break;
            }
            ++id;
            id %= cap;
        }
        if (Data[id].size() == 0) {
            insert(std::make_pair(key, ValueType()));
            return (*find(key)).second;
        }
        return (*(Data[id][0])).second;
    }

    const ValueType& at(const KeyType& key) const {
        size_t id = my_H(key) % cap;
        while (Data[id].size() != 0) {
            if (Data[id].size() != 2 && (*Data[id][0]).first == key) {
                break;
            }
            ++id;
            id %= cap;
        }
        if (Data[id].size() == 0) {
            throw std::out_of_range("Out Of Range");
        }
        return (*(Data[id][0])).second;
    }

    void clear() {
        Data.clear();
        Data.resize(2);
        cap = 2;
        count_el = 0;
        all_elems.clear();
    }
};

