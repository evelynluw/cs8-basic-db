/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#ifndef INC_0514_BPTREE_MAP_H
#define INC_0514_BPTREE_MAP_H

#include "BPTree.h"

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()): key(k), value(v) {}
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me) {
        outs << print_me.key << ": " << print_me.value << ", ";
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key <= rhs.key;
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        Pair<K, V> temp = lhs;
        temp.value = rhs.value;
        return temp;
    }
};

template <typename K, typename V>
class Map
{
public:
    typedef BPTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(): _it() {};
        Iterator(typename map_base::Iterator it): _it(it) {}
        Iterator operator ++(int unused) {
            return _it++;
        }
        Iterator operator ++() {
            return ++_it;
        }
        Pair<K, V> operator *() {
            return *_it;
        }
        Pair<K, V>* operator->() {
            return _it.operator->();
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

        bool is_null() const {
            return _it.is_null();
        }

        void print_Iterator(ostream& outs = cout) const {
            _it.print_Iterator(outs);
        }
    private:
        typename map_base::Iterator _it;
    };

    Map();
    Map(const Map& other);
//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const V& operator[](const K& key) const;
    V& operator[](const K& key);
    const V& at(const K& key) const;
    V& at(const K& key);


//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    Iterator find_get(const K& key);
    bool contains(const Pair<K, V>& target) const;

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BPTree<Pair<K, V> > map;
};

template<typename K, typename V>
Map<K, V>::Map(): key_count(), map() {

}

template<typename K, typename V>
Map<K, V>::Map(const Map &other): key_count(other.key_count), map(other.map) {

}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin() {
    return Iterator(map.begin());
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end() {
    return Iterator(map.end());
}

template<typename K, typename V>
int Map<K, V>::size() const {
    return map.size();
}

template<typename K, typename V>
bool Map<K, V>::empty() const {
    return map.empty();
}

template<typename K, typename V>
const V &Map<K, V>::operator[](const K &key) const {
    Map<K, V>::Iterator it = find(key);
    return it->value;
}

template<typename K, typename V>
V &Map<K, V>::operator[](const K &key) {
    Map<K, V>::Iterator it = find(key);
    if(it.is_null()) {
        map.insert(Pair<K, V>(key));
        it = find(key);
    }
    return it->value;
}

template<typename K, typename V>
const V &Map<K, V>::at(const K &key) const {
    Map<K, V>::Iterator it = find(key);
    return it->value;
}

template<typename K, typename V>
V &Map<K, V>::at(const K &key) {
    Map<K, V>::Iterator it = find(key);
    if(it.is_null()) {
        map.insert(Pair<K, V>(key));
        it = find(key);
    }
    return it->value;
}

template<typename K, typename V>
void Map<K, V>::insert(const K &k, const V &v) {
    map.insert(Pair<K, V>(k ,v));
}

template<typename K, typename V>
void Map<K, V>::erase(const K &key) {
    map.remove(Pair<K, V>(key));
}

template<typename K, typename V>
void Map<K, V>::clear() {
    map.clear_tree();
}

template<typename K, typename V>
V Map<K, V>::get(const K &key) {
    return map.get(Pair<K, V>(key));
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K &key) {
    return Iterator(map.find(Pair<K, V>(key)));
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find_get(const K &key) {
    return Iterator(map.find_ge(Pair<K, V>(key)));
}

template<typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V> &target) const {
//    Iterator it = find(target.key);
//    if(!it.is_null()) {
//        if(target.value == it->value)
//            return true;
//    }
//    return false;
    return map.contains(target);
}

#endif //INC_0514_BPTREE_MAP_H
