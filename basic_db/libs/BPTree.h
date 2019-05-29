/*
 * Author: Evelyn Lu
 * Project: 0430_BTree
 * Purpose: build a btree based on the
 * File: BPTree.h definitions for BPTree class
*/

#ifndef INC_0430_BTREE_BTREE_H
#define INC_0430_BTREE_BTREE_H

#include <iostream>
#include <iomanip>
#include <cassert>
#include "array_utilities.h"

template<class T>
class BPTree {
public:
    class Iterator {   // all defined in line according to instructions
    public:
        friend class BPTree;
        Iterator(BPTree<T>* _ptr=NULL, int _key_num = 0):
            ptr(_ptr), key_num(_key_num){}
        //! dereference operator
        T operator *(){
            assert((!is_null()) && key_num < ptr->data_count);
            return ptr->data[key_num];
        }
        //! member access operator
        T* operator->() {
            assert(!is_null());
            return &(ptr->data[key_num]);
        }

        //! `++it`
        Iterator operator++(){
            if (!is_null()) {
                if (key_num == ptr->data_count-1) {// last data in the node
                    ptr = ptr->next;
                    key_num = 0;
                }
                else
                    key_num++;
            }
            return *this;
        }

        friend Iterator operator++(Iterator &it, int un_used){
           return ++it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs.ptr == rhs.ptr && lhs.key_num == rhs.key_num;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs == rhs);
        }
        //! assume this function prints out the data according to the iterator
        void print_Iterator(ostream& outs = cout) const{
            Iterator temp = *this;
            while (!temp.is_null()) {
                outs << temp.ptr->data[temp.key_num] << " ";
                temp++;
            }
        }
        bool is_null() const {return !ptr;}
    private:
        BPTree<T>* ptr;
        int key_num; // the # of the data in the array
    };

    //big three:
    BPTree(bool dups = false);
    BPTree(const BPTree<T> &other);
    ~BPTree();

    /*! per my current understanding, the iterator points to each value in the
     * leaf node. The `ptr` means the pointer for the node, and the `key_num`
     * which is in int should be the # of the data the iterator is actually
     * pointing to in the data array inside the node.
     */

    //! (DEEP copying)
    BPTree<T> &operator=(const BPTree<T> &rhs);

    // Mutators
    bool insert(const T &entry);
    bool remove(const T &entry);
    void clear_tree();

    // Accessors
    void copy_tree(const BPTree<T> &other);
    void copy_tree_shallow(const BPTree<T> &other);
    bool contains(const T &entry) const;
    T &get(const T &entry);
    T& get_existing(const T& entry);
    T *find_T(const T &entry);
    Iterator find(const T& entry);
    Iterator find_ge(const T& entry);
    Iterator begin();
    Iterator end();
    size_t size() const;
    bool empty() const;

    void print(ostream &outs = cout, size_t level = 0) const;
    void print_debug(ostream &outs = cout, size_t level = 0) const;
    void print_tree(int level = 0, ostream &outs = cout) const;
    void print_list(ostream &outs = cout) const;
    friend ostream &operator<<(ostream &outs, const BPTree<T> &print_me) {
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    static const bool dups_ok = false;                                   //true if duplicate keys may be inserted
    size_t data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    size_t child_count;                                //number of children
    BPTree *subset[MAXIMUM + 2];                     //subtrees
    BPTree* next;
    bool is_leaf() const;
    bool loose_insert(const T &entry);
    void fix_excess(size_t i);
    bool loose_remove(const T &entry);
    void fix_shortage(int i);
    void remove_biggest(T &removed_entry);
    T get_smallest();
    BPTree* get_smallest_leaf_node();
    BPTree* get_biggest_leaf_node();
    void update(size_t i);
    void rotate_left(int i);
    void rotate_right(int i);
    void merge_with_next_subset(size_t i);
    bool check_if_valid(BPTree<T>* head);

};

template<class T>
BPTree<T>::BPTree(bool dups): data_count(), child_count(), next(nullptr) {
    for (size_t i = 0; i < MAXIMUM + 2; ++i) {
        subset[i] = nullptr;
    }
}

template<class T>
BPTree<T>::BPTree(const BPTree<T> &other): data_count(other.data_count),
                                           child_count(other.child_count),
                                           next(other.next) {
    for (size_t i = 0; i < MAXIMUM + 2; ++i) {
        subset[i] = nullptr;
    }
    *this = other;
}

template<class T>
BPTree<T>::~BPTree() {
    clear_tree();
}

//! (DEEP copying)
template<class T>
BPTree<T> &BPTree<T>::operator=(const BPTree<T> &rhs) {
    copy_tree(rhs);
    return *this;
}

//! insert the entry to the tree.
//! \return true if entry is new, false if entry already exists
template<class T>
bool BPTree<T>::insert(const T &entry) { //tested
    /*
    1. do loose insert to insert to where the item belongs and forget about
     the capacity; then inside loose insert call fix_excess.
    2. after that the head might be full. so we "clone" the current head
     as its 0th subset, and call fix_excess on that.
     */
    if(!loose_insert(entry))
        return false;
    if(data_count == MAXIMUM + 1) {
        BPTree<T>* new_child = new BPTree<T>;
//            (*new_child) = *this;
        new_child->copy_tree_shallow(*this);
        data_count = 0;
        child_count = 1;
        subset[0] = new_child;
        fix_excess(0);
    }
    return true;
}

/*! Remove entry from the tree.
 * After loose_remove, the root might be empty and have a single subset.
 * Then the tree must shrink, by creating a temp = subset[0] and
 * shallow copying the temp into "this." However, the temp->child_count
 * needs to be set to 0 before its destructor runs.
 * \return false if not found, true if found and removed
 * \param entry the entry to be removed
 */
template<class T>
bool BPTree<T>::remove(const T &entry) {
    // still a bug! check work report
    if(!loose_remove(entry)) return false;
    if(data_count < MINIMUM) {
        // head is empty. now: copy the data and children
        //  from subset[0] to this; then delete the pointer to
        //  subset[0].kdr
        BPTree<T>* temp = subset[0];
//            *this = *temp;
        copy_tree_shallow(*temp);
        temp->data_count = 0;
        temp->child_count = 0;
        delete temp;
    }
    return true;
}

//! set data_count and child_count to 0;
//! delete the subsets.
template<class T>
void BPTree<T>::clear_tree() {
//        cout << this << " CLEAR_TREE FIRED" << endl;
    for (size_t i = 0; i < data_count; ++i) {
        data[i] = T();
    }
    data_count = 0;
    for (size_t i = 0; i < child_count; ++i) {
        subset[i]->clear_tree();
        delete subset[i];
        subset[i] = nullptr;
    }
    child_count = 0;
    // what if the subsets outside of child_count still
    // exists? e.g. child_count == 0 but subset[0] is
    // not NULL
}

//! copy other into this object. (DEEP copying)
template<class T>
void BPTree<T>::copy_tree(const BPTree<T> &other) {
    copy_array(data, other.data, data_count, other.data_count);
    child_count = other.child_count;
    for (size_t i = 0; i < other.child_count; ++i) {
        BPTree<T>* copy_child = new BPTree<T>;
        copy_child->copy_tree(*(other.subset[i]));
        subset[i] = copy_child;
    }
    // now let's deal with "next"!
    if(!is_leaf()) {
        if (subset[0]->is_leaf()) {
            // direct parent of leaves: connect the "next"
            // of children leaf nodes
            for (size_t i = 0; i < child_count - 1; ++i) {
                subset[i]->next = subset[i+1];
            }
        }
        else {
            // indirect parents of leaves:
            // connect "next" of biggest on the left
            // and smallest on the right.
            for (size_t i = 0; i < child_count - 1; ++i) {
                subset[i]->get_biggest_leaf_node()->next =
                        subset[i+1]->get_smallest_leaf_node();
            }
        }
    }
}

template<class T>
void BPTree<T>::copy_tree_shallow(const BPTree<T> &other) {
    copy_array(data, other.data, data_count, other.data_count);
    copy_array(subset, other.subset, child_count, other.child_count);
}

//true if entry can be found in the array
template<class T>
bool BPTree<T>::contains(const T &entry) const {
    size_t i = first_ge(data, data_count, entry);
    if(data[i] == entry) {
        return true;
    }
    else if (is_leaf()) {//shouldn't happen
        return false;
    }
    else {
        return subset[i]->contains(entry);
    }
}

/*!
 * \return a reference to entry in the tree.
 * If entry is not in the tree, CRASH (assert this condition).
 */
template<class T>
T &BPTree<T>::get(const T &entry) { //tested
    // "CRASH" if the entry is not in the tree?
    // THIS IS EXTRA STEPS
    assert(contains(entry));
    size_t i = first_ge(data, data_count, entry);
    if(data[i] == entry) {
        return data[i];
    }
    else if (is_leaf()) {//shouldn't happen
        cout << "CONTAINS CHECK FAILED: ENTRY NOT IN TREE" << endl;
    }
    else {
        return subset[i]->get(entry);
    }

}

/*! \return a pointer to the entry if found in the tree;
 * otherwise returns NULL.
 */
template<class T>
T *BPTree<T>::find_T(const T &entry) { //tested
    // "CRASH" if the entry is not in the tree?
    // THIS IS EXTRA STEPS
    assert(contains(entry));
    size_t i = first_ge(data, data_count, entry);
    if(data[i] == entry) {
        return &(data[i]);
    }
    else if (is_leaf()) {//shouldn't happen
        cout << "CONTAINS CHECK FAILED: ENTRY NOT IN TREE" << endl;
    }
    else {
        return subset[i]->find_T(entry);
    }
}

template<class T>
typename BPTree<T>::Iterator BPTree<T>::find(const T &entry){
    size_t i = first_ge(data, data_count, entry);
    if(is_leaf()) {
        if(i < data_count && data[i] == entry) {  //found
            return Iterator(this, i);
        } else {                //not found
            return Iterator();
        }
    }
    else {
        if(i < data_count && data[i] == entry) {
            return subset[i+1]->find(entry);
        }
        return subset[i]->find(entry);
    }
}

template<class T>
typename BPTree<T>::Iterator BPTree<T>::find_ge(const T &entry) {
    size_t i = first_ge(data, data_count, entry);
    if(is_leaf()) {
        if(i < data_count)
            return Iterator(this, i);
        else
            return Iterator(this->next, 0);
    }
    else {
        if(i < data_count && data[i] == entry) {
            return subset[i+1]->find(entry);
        }
        return subset[i]->find_ge(entry);
    }
}

template<class T>
typename BPTree<T>::Iterator BPTree<T>::begin() {
    auto temp = this;
    while(!temp->is_leaf()) {
        temp = temp->subset[0];
    }
    //now temp should be the first leaf node.
    // the first data of temp should be the beginning of this tree.
    // return this iterator.
    return Iterator(temp, 0);
}

template<class T>
typename BPTree<T>::Iterator BPTree<T>::end() {
    return Iterator(nullptr);
}

//! \return the number of elements in the tree
template<class T>
size_t BPTree<T>::size() const {
    auto temp = this;
    while(!temp->is_leaf()) {
        temp = temp->subset[0];
    }
    //now temp should be the first leaf node
    size_t size = 0;
    while(temp!= nullptr) {
        size += temp->data_count;
        temp = temp->next;
    }
    return size;

}


// true if the tree is empty
template<class T>
bool BPTree<T>::empty() const {
    //the head can never be empty except when the whole tree
    // is empty
    return data_count == 0;
}

//! "vertical" printing
template<class T>
void BPTree<T>::print(ostream &outs, size_t level) const {
    if(is_leaf()) {
        outs << setw(level * 4) << "|";
        for (size_t i = 0; i < data_count; ++i) {
            outs << data[i];
            if(i != data_count - 1)
                outs << ", ";
        }
        outs << "|" << endl;
    }
    for (size_t i = child_count - 1; i != SIZE_MAX; --i) {
        subset[i]->print(outs, level + 1);
        if(i - 1 != SIZE_MAX)
            outs << setw(level*4) << "|" << data[i - 1] << "|" << endl;
    }
}

template<class T>
void BPTree<T>::print_debug(ostream &outs, size_t level) const {
    if(is_leaf()) {
        outs << setw(level * 4) << "|";
        for (size_t i = 0; i < data_count; ++i) {
            outs << data[i];
            if(i != data_count - 1)
                outs << ", ";
        }
        outs << "| " << this;
        outs << endl;
    }
    for (size_t i = child_count-1; i != SIZE_MAX; --i) {
        subset[i]->print_debug(outs, level+1);
        if (i-1 != SIZE_MAX) {
            outs << setw(level*4) << "|" << data[i-1] << "| ";
        }
        if (i-1 == 0)
            outs << this;
        outs << endl;
    }
}


//! print a readable version of the tree
template<class T>
void BPTree<T>::print_tree(int level, ostream &outs) const {
    //now use the same algo as the regular print
    print(outs, level);
}

template<class T>
void BPTree<T>::print_list(ostream &outs) const {
    auto temp = this;
    while(!temp->is_leaf()) {
        temp = temp->subset[0];
    }
    //now temp should be the first leaf node
    while(temp!= nullptr) {
        print_array(temp->data, temp->data_count, -1);
        temp = temp->next;
    }
    outs << endl;
}

template<class T>
bool BPTree<T>::is_valid() {
    // 1: The root may have as few as one entry (or even no
    // entries if it also has no children); every other node has at least
    // MINIMUM entries.
    // 2: The maximum number of entries in a node is twice the
    // value of MINIMUM.
    // 3: The entries of each B-tree node are stored in a
    // partially filled array, sorted from the smallest entry (at index 0)
    // to the largest entry (at the final used position of the array).
    // 4: The number of subtrees below a nonleaf node is always
    // one more than the number of entries in the node.
    // 5: For any nonleaf node:
    // (a) An entry at index i is greater than all the entries in subtree
    // number i of the node, and
    // (b) an entry at index i is less than all the entries in subtree
    // number of the node.

    if(is_leaf()) {
        return data_count == 0;
    }

    if(data_count < 1) {
        return false;
    }

    for (size_t i = 0; i < child_count; ++i) {
        if(!check_if_valid(subset[i]))
            return false;
    }
    return true;
}

// PRIVATE FUNCTIONS

//! \return true if this is a leaf node
template<class T>
bool BPTree<T>::is_leaf() const { return child_count == 0; }


/*! \return true if the entry has been added to the tree; false if entry
 * already exists.
 * \pre The entire B-tree is valid.
 * \post If entry was already in the set, then the set is unchanged
 * and the return value is false. Otherwise, the entry has been added to the
 * set, the return value is true, and the entire B-tree is still
 * valid EXCEPT that the number of entries in the root of this set might be
 * one more than the allowed maximum (which will be fixed in the
 * insert function)
 */
template<class T>
bool BPTree<T>::loose_insert(const T &entry) { //tested
    /*
     *  four cases:
     *  1. found / leaf: call overloaded +
     *  2. found / !leaf call subset[i+1]->loose_insert and fix_excess(i+1)
     *  3. !found / leaf insert entry at position data[i]
     *  4. !found / !leaf call subset[i]->loose_insert and fix_excess(i)
     */
    size_t i = first_ge(data, data_count, entry);
    if (i < data_count && data[i] == entry) // i or i-1?
    {
        if (is_leaf()) {        // case 1: found / leaf
            //NOTE: the T data type must have a properly overloaded operator +.
            //or repeated entries will add up. Resulting in fatal errors!
            data[i] = data[i] + entry;   //Warning!!!
            return false;
        } else {                // case 2: found / !leaf
            subset[i+1]->loose_insert(entry);
            fix_excess(i+1);
        }
    } else if (is_leaf()) {     // case 3: !found / leaf
        //efficiency issues.. calling first_ge twice
        //use the index insert?
        ordered_insert(data, data_count, entry);
        return true;
    } else {                    // case 4: !found / !leaf
        bool state = subset[i]->loose_insert(entry);
        fix_excess(i);
        return state;
    }
    return true;
}

/*!
 * \pre subset[i]->data may be full, and the capacity of data
 * is an odd number
 * \post if subset[i] is full, split it into two children nodes (insert the
 * latter to the position after the original node); insert the middle to
 * "this"
 */
template<class T>
void BPTree<T>::fix_excess(size_t i) { //tested
    T mid;
    if (subset[i]->data_count == MAXIMUM + 1) {
        if (get_middle(subset[i]->data, subset[i]->data_count, mid))
            ordered_insert(this->data, data_count, mid);
        BPTree<T>* temp = new BPTree;
        split(subset[i]->data, subset[i]->data_count,
              temp->data, temp->data_count);
        split(subset[i]->subset, subset[i]->child_count,
              temp ->subset, temp->child_count);
        //if the split nodes are leaf nodes...
        // insert middle item up to data[i]
        if(subset[i]->is_leaf())
            insert_item(temp->data, 0, temp->data_count, mid);
        //point the "next" of the left split to the right split
        temp->next = subset[i]->next;
        subset[i]->next = temp;
        //now this split temp node is at the right of subset[i]
        insert_item(subset, i + 1, child_count, temp);
    }
}
//fix excess of data elements in child i

//! remove despite whatever then call fix_shortage
//! \return false if not found, true if found and removed
template<class T>
bool BPTree<T>::loose_remove(const T &entry) {
    /* four cases:
    a. leaf && not found target: there is nothing to do
    b. leaf && found target: just remove the target
    c. not leaf and not found target: recursive call to loose_remove
    d. not leaf and found: replace target with largest child of subset[i]
          | !found      | found         |
    ------|-------------|---------------|-------
     leaf | a: nothing  | b: delete     |
          | to do       | target        |
    ------|-------------|---------------|-------
    !leaf | c: loose_   | d: replace    |
          | remove      | w/ biggest    |
    ------|-------------|---------------|-------
    */
    size_t i = first_ge(data, data_count, entry);
    if(is_leaf()) {
        if (data[i] == entry){ //found & leaf: delete it
            delete_item(data, i, data_count);
        } else { //!found & leaf: nothing to do
            return false;
        }
    }
    else {
        if (data[i] == entry && i < data_count) { //found & not leaf: replace
            subset[i+1]->loose_remove(entry);
            if (subset[i+1]->data_count < MINIMUM)
                fix_shortage(i+1);
            if(data[i] == entry)
                update(i);
            else{
//                subset[i]->data[subset[i]->data_count-1]
//                    = subset[i]->get_smallest();
                subset[i]->update(subset[i]->data_count-1);
                // experimental
            }
        }
        else {
            subset[i]->loose_remove(entry);
            if (subset[i]->data_count < MINIMUM)
                fix_shortage(i);
//            if(i>0)
//                data[i-1] = subset[i]->get_smallest();
        }
    }
    return true;
}

/*! allows MINIMUM-1 data elements in the root.
 * @param i index of the short subset
 */
template<class T>
void BPTree<T>::fix_shortage(int i) {
    //different when the subset nodes are LEAF NODES.
    //check leaf nodes during deletion... inside rotate and merge!
    if (i > 0 && subset[i-1]->data_count > MINIMUM) {
        rotate_right(i);
    }
    else if (i < child_count - 1 && subset[i+1]->data_count > MINIMUM) {
        rotate_left(i);
    }
    else if (i > 0 && subset[i-1]->data_count == MINIMUM) {
        // merge left & top
        if(!subset[i]->is_leaf()){
            attach_item(subset[i-1]->data, subset[i-1]->data_count, data[i-1]);
//            delete_item(data, i-1, data_count);
        }
        delete_item(data, i-1, data_count);
        merge_with_next_subset(i-1);
    }
    else if (i < child_count - 1 && subset[i+1]->data_count == MINIMUM) {
        // merge right & top
        if(!subset[i]->is_leaf()){
            insert_item(subset[i+1]->data, 0, subset[i+1]->data_count, data[i]);
        }
        delete_item(data, i, data_count);
        merge_with_next_subset(i);
    }
}

/*! fix shortage of data elements in child i.
 * \pre remove() is being called and the target is found, however
 * the node target "root" is in has children.
 * \post the biggest entry is removed in all subsets under subset[i]
 * of the "root" and placed where the target was. fix_shortage() should
 * be called afterwards.
 * \param removed_entry the biggest item removed from the entry.
 */
template<class T>
void BPTree<T>::remove_biggest(T &removed_entry) {
    if (is_leaf()) {
        removed_entry = data[--data_count];
        return;
    }
    subset[child_count - 1]->remove_biggest(removed_entry);
    if (subset[child_count-1]->data_count < MINIMUM)
        fix_shortage(child_count-1);
}

/*!
 * get the smallest data from this branch. This number should be equal to the
 * parent of this branch.
 */
template<class T>
T BPTree<T>::get_smallest() {
    if(is_leaf()) {
        return data[0];
    }
    return subset[0]->get_smallest();
}

template<class T>
BPTree<T> *BPTree<T>::get_smallest_leaf_node() {
    if (is_leaf())
        return this;
    return subset[0]->get_smallest_leaf_node();
}

template<class T>
BPTree<T> *BPTree<T>::get_biggest_leaf_node() {
    if (is_leaf())
        return this;
    return subset[child_count - 1]->get_biggest_leaf_node();
}

template<class T>
void BPTree<T>::update(size_t i) {
    if(is_leaf())
        return;
    data[i] = subset[i+1]->get_smallest();
}


//remove the biggest child of this tree->entry
//OK with leaf nodes
template<class T>
void BPTree<T>::rotate_left(int i) {
    //this will be different when the bottom nodes are leaf nodes
    // step 1. rotate from top to the short node
    attach_item(subset[i]->data, subset[i]->data_count, data[i]);
    // step 2. rotate right node to the top
    T temp;
    delete_item(subset[i+1]->data, 0,
                subset[i+1]->data_count, temp);
    if(subset[i]->is_leaf())
        data[i] = subset[i+1]->data[0];
    else
        data[i] = temp;
    // step 3. take care of first child of right node
    if (subset[i+1]->child_count > 0) {
        BPTree<T> *first_child;
        delete_item(subset[i+1]->subset, 0,
                    subset[i+1]->child_count, first_child);
        attach_item(subset[i]->subset, subset[i]->child_count, first_child);
    }
}
//transfer one element LEFT from child i
template<class T>
void BPTree<T>::rotate_right(int i) {
    // step 1. rotate from top to the short node
    if(!subset[i]->is_leaf())
        insert_item(subset[i]->data, 0, subset[i]->data_count, data[i-1]);

    // step 2. rotate left node to the top
    T temp;
    delete_item(subset[i-1]->data, subset[i-1]->data_count - 1,
                subset[i-1]->data_count, temp);
    if(subset[i-1]->is_leaf()) {
        insert_item(subset[i]->data, 0, subset[i]->data_count, temp);
        data[i-1] = subset[i]->data[0];
    }
    else
        data[i-1] = temp;
    // step 3. take care of last child of left node
    if (subset[i-1]->child_count > 0) {
        BPTree<T>* last_child;
        delete_item(subset[i-1]->subset, subset[i-1]->child_count-1,
                    subset[i-1]->child_count, last_child);
        insert_item(subset[i]->subset, 0, subset[i]->child_count,
                    last_child);
    }
}

//merge subset i with subset i+1
template<class T>
void BPTree<T>::merge_with_next_subset(size_t i) {
    merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
    merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
    delete_item(subset, i+1, child_count);
}

template<class T>
bool BPTree<T>::check_if_valid(BPTree<T> *head) {
    if (head->data_count < MINIMUM || head->data_count > MAXIMUM)
        return false;
    if (head->is_leaf())
        return true;
    if (head->child_count != head->data_count+1) {
        return false;
    }
    if (!is_sorted(head->data, head->data_count)) {
        return false;
    }
    for (size_t i = 0; i < head->child_count; ++i) {
        if(!check_if_valid(head->subset[i]))
            return false;
    }
    return true;
}

#endif //INC_0430_BTREE_BTREE_H
