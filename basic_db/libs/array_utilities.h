/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#ifndef INC_0430_BTREE_ARRAY_UTILITIES_H
#define INC_0430_BTREE_ARRAY_UTILITIES_H

#include <iostream>
#include <vector>

using namespace std;

//template <class T>
//void copy_array(T dest[], const T src[],
//                size_t & dest_size, size_t src_size);
//template <class T>
//void copy_array(T *dest, const T *src, size_t &size);

/* Precondition of these functions:
 * The n is either data_count or child_count. So, it will
 * be smaller than the capacity. (except for special
 * situations, like copy)
 * The array has capacity that is always bigger than n.
 */

//! print array data
template <class T>
void print_array(const T data[], size_t n, int pos = -1, ostream &outs = cout) { //OK
    //pos = -1 doesnt seem like a good idea
    for (size_t i = pos + 1; i < n; ++i) {
        outs << data[i] << ' ';
    }
//    outs << endl;
}

//! return the larger of the two items
template <class T>
T maximal(const T& a, const T& b) { //OK
    return (a>b) ? a : b;
}

//! swap the two items
template <class T>
void swap(T& a, T& b) { //OK
    T temp = a;
    a = b;
    b = temp;
}

//! return index of the largest item in data
template <class T>
size_t index_of_maximal(T data[ ], size_t n) { //OK
    return n - 1;
}

//! return the index of first element in data that is
//! greater or equal to (not less than) entry
template <class T>
size_t first_ge(const T data[ ], size_t n, const T& entry) { //OK
//    //binary search later
//    size_t i;
//    for (i= 0; i < n && data[i] < entry; ++i);
//    return i;

    //binary search: iteration version
    if(n == 0)
        return 0;
    size_t low = 0, high = n - 1, mid;
    while(low <= high) {
        mid = low + (high - low)/2;
        if(data[mid] < entry) { //continue moving right
             low = mid + 1;
        } else {
            if(mid == 0)
                return mid;
            if(data[mid - 1] < entry)
                return mid;
            high = mid - 1;
        }
    }

    return n;
 }

//! append entry to the right of data
template <class T>
void attach_item(T data[ ], size_t & n, const T& entry) { //OK
    data[n] = entry;
    n++;
}

//! insert entry at index i in data
template <class T>
void insert_item(T data[ ], size_t index, size_t & n, T entry) { //OK
    for (size_t j = n + 1; j > index; --j)
        data[j] = data[j - 1];
    data[index] = entry;
    n++;
}

//! delete item at index i and place it in entry
template <class T>
void delete_item(T data[ ], size_t i, size_t & n, T& entry) { //OK
    entry = data[i];
    data[i] = T();
    for (size_t j = i + 1; j < n; ++j) {
        data[j-1] = data[j];
    }
    n--;
}

//! delete item at index i
template <class T>
void delete_item(T data[ ], size_t i, size_t & n) { //OK
    data[i] = T();
    for (size_t j = i + 1; j < n; ++j) {
        data[j-1] = data[j];
    }
    n--;
}


//! remove the last element in data and place
//! it in entry
template <class T>
void detach_item(T data[], size_t & n, T& entry) { //OK
    entry = data[n];
    data[n] = 0;
    n--;
}

//! copy src[] into dest[]
template <class T>
void copy_array(T dest[], const T src[],
                size_t & dest_size, size_t src_size) {
    for (size_t i = 0; i < src_size; ++i) {
        dest[i] = src[i];
    }
    dest_size = src_size;
}

//template<class T>
//void copy_array(T *dest, const T *src, size_t &size) {
//    for (size_t i = 0; i < size; ++i)
//        dest[i] = src[i];
//}

//! append "from" to the right of "to"
template <class T>
void merge(T to[ ], size_t & to_size, T from[ ], size_t & from_size) {
//    copy_array(to + to_size, from, to_size, from_size);
    //this part makes to_size = from_size? not good
    for (size_t i = 0; i < from_size; ++i) {
        to[i+to_size] = from[i];
    }
    to_size += from_size;
}

//! move n/2 elements from the right of data1
//! and move to data2
template <class T>
void split(T data1[ ], size_t & n1, T data2[ ], size_t & n2) {
    copy_array(data2, data1+(n1+1)/2, n2, n1/2);
    n1 = n1/2;
}

//! return true if array # is odd, which has a mid and mid = mid value,
//! return false if array # is even
template <class T>
bool get_middle(T data[], size_t n, T &mid) {
    if(n%2 == 0)
        return false;
    mid = data[n/2];
    return true;
}

//! item > all data[i]
template <class T>
bool is_gt(const T data[], size_t n, const T& item) {
    return 0;
}

//! item <= all data[i]
template <class T>
bool is_le(const T data[], size_t n, const T& item) {
    return 0;
}

//! insert entry into the sorted array
//! data with length n
template <class T>
void ordered_insert(T data[], size_t &n, const T &entry) { //OK
    //should use binary search?
    //dont worry about m = n exception yet?
    size_t i = first_ge(data, n, entry);
    //move the bigger items one spot to the right
    //size n stays constant
    for (size_t j = n/* + 1*/; j > i; --j)
        data[j] = data[j - 1];
    data[i] = entry;
    n++;
}

template <class T>
bool is_sorted(const T *a, size_t n) {
    for (size_t i = 0; i < n - 1; ++i) {
        if(a[i]>a[i+1]){
            return false;
        }
    }
    return true;
}

//-------------- Vector Extra operators: ---------------------

//! print vector list
template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) {
    for (auto it = list.begin(); it != list.end(); ++it)
        outs << *it << ", ";
    return outs;
}

//! list.push_back addme
template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) {
    list.push_back(addme);
    return list;
}



#endif //INC_0430_BTREE_ARRAY_UTILITIES_H
