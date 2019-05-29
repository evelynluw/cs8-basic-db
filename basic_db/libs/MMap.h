/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#ifndef INC_0514_BPTREE_MMAP_H
#define INC_0514_BPTREE_MMAP_H
#include <vector>
#include "BPTree.h"

template <class V>
void print_vector(vector<V> v, ostream& outs = cout) {
    for (auto it = v.begin(); it != v.end(); ++it)
        outs << *it << ", ";
    outs << endl;
}


template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()): key(k) {}
    MPair(const K& k, const V& v): key(k), value_list() {
        value_list.insert(value_list.begin(), v);
    }
    MPair(const K& k, const vector<V>& vlist): key(k), value_list(vlist) {}
    MPair(const MPair& other): key(other.key), value_list(other.value_list) {}
    //--------------------------------------------------------------------------------

    //You'll need to overload << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {
        outs << print_me.key << ": ";
//        print_me.print_vector();
        outs << print_me.value_list;
        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key == rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key <= rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        // expanding the vector of lhs?
        // lhs += rhs
        MPair<K, V> temp = lhs;
        for(typename vector<V>::const_iterator it = rhs.value_list.begin();
            it != rhs.value_list.end(); ++ it) {
            temp.value_list.push_back(*it);
        }
        return temp;
    }

    MPair<K, V>& operator+=(const MPair<K, V>& rhs) {
        for(typename vector<V>::const_iterator it = rhs.value_list.begin();
            it != rhs.value_list.end(); ++ it) {
            value_list.push_back(*it);
        }
        return *this;
    }

    MPair<K, V>& operator+=(const V rhs) {
        value_list.push_back(rhs);
        return *this;
    }

    void print_vector(ostream& outs = cout) const {
        for (auto it = value_list.begin(); it != value_list.end(); ++it)
            outs << *it << ", ";
//        outs << endl;
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPTree<MPair<K, V> > map_base;
    class Iterator{ // defined inline
    public:
        friend class MMap;
        //! construct with BPTree Iterator
        Iterator(): _it() {};
        Iterator(typename map_base::Iterator it): _it(it) {};
        Iterator operator ++(int unused) {
            _it++;
        }
        Iterator operator ++() {
            ++_it;
        }
        MPair<K, V> operator *() {
            return *_it;
        }

        MPair<K, V>* operator->() const {
            return _it.operator->();
        }

        MPair<K, V>* operator->() {
            return _it.operator->();
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

        void print_Iterator(ostream& outs = cout) {
            _it.print_Iterator(outs);
        }

        bool is_null() const {
            return _it.is_null();
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap();
    MMap(const MMap& other);
    MMap<K, V> &operator=(const MMap& rhs);

//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const ;
    vector<V> &get(const K& key);

    Iterator find(const K& key);
    Iterator find_ge(const K& key);
    int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:



    bool is_valid();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

    void print_list(ostream& outs = cout) {mmap.print_list(outs);}
    void print_graph() {mmap.print();}

private:
    BPTree<MPair<K, V> > mmap;
};

//DEFINTIONS

template<typename K, typename V>
MMap<K, V>::MMap(): mmap() {}

template<typename K, typename V>
MMap<K, V>::MMap(const MMap &other): mmap(other.mmap) {}

template<typename K, typename V>
MMap<K, V> &MMap<K, V>::operator=(const MMap &rhs) {
    mmap = rhs.mmap;
    return *this;
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K, V>::begin() {
    return MMap::Iterator(mmap.begin());
}

template<typename K, typename V>
typename MMap<K,V>::Iterator MMap<K, V>::end() {
    return MMap::Iterator(mmap.end());
}

template<typename K, typename V>
int MMap<K, V>::size() const {
    return mmap.size();
}

template<typename K, typename V>
bool MMap<K, V>::empty() const {
    return mmap.empty();
}

template<typename K, typename V>
const vector<V> &MMap<K, V>::operator[](const K &key) const {
    MMap<K, V>::Iterator it = find(key);
    //what to do when the key cannot be found? ...
//    if(it.is_null())
//        return vector<V>(0);
    return it->value_list;
}

template<typename K, typename V>
vector<V> &MMap<K, V>::operator[](const K &key) {
    MMap<K, V>::Iterator it = find(key);
    // in this case when the key is not found:
    // insert it!
    if(it.is_null()) {
        mmap.insert(MPair<K, V>(key));
        it = find(key);
    }
    return it->value_list;
}

template<typename K, typename V>
void MMap<K, V>::insert(const K &k, const V &v) {
    mmap.insert(MPair<K, V>(k, v));
}

//! erase the whole mpair? Or which one to erase?
template<typename K, typename V>
void MMap<K, V>::erase(const K &key) {
    mmap.remove(MPair<K, V>(key));
}

template<typename K, typename V>
void MMap<K, V>::clear() {
    mmap.clear_tree();
}

template<typename K, typename V>
bool MMap<K, V>::contains(const K &key) const {
    return mmap.contains(key);
}

template<typename K, typename V>
vector<V> &MMap<K, V>::get(const K &key) {
    return mmap.get(MPair<K, V>(key)).value_list;
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::find(const K &key) {
    return Iterator(mmap.find(MPair<K, V>(key)));
}

template<typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::find_ge(const K &key) {
    return Iterator(mmap.find_ge(MPair<K, V>(key)));
}

template<typename K, typename V>
int MMap<K, V>::count(const K &key) {
    return mmap.size();
}

template<typename K, typename V>
bool MMap<K, V>::is_valid() {
    return mmap.is_valid();
}

#endif //INC_0514_BPTREE_MMAP_H
