/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#include <stack>
#include <bits/stdc++.h>
#include "Table.h"

using namespace std;

Table::Table(): _table_name(), _table() {
}

Table::Table(string name): _table_name(name), _table() {
}

Table::Table(string name, vector<string> fields): _table_name(name),
    _fields(fields) {
    for (size_t i = 0; i < fields.size(); ++i) {
        _fields_map.insert(fields[i], i);
    }
    for (auto it = _fields.begin(); it != _fields.end(); ++it) {
        _table.push_back(MMap<string, int>());
    }
}
void Table::insert_into(vector<string> values, size_t rec_no) {
    for (size_t i = 0; i < values.size(); ++i) {
        _table[i].insert(values[i], rec_no);
    }
}

vector<int> Table::select_record_numbers(MMap<string, string> _parsed) {
    for (auto & it : _parsed["LOGIC"]) {
        transform(it.begin(), it.end(), it.begin(), ::toupper);
    }
    stack<string> op_stk;
    vector<vector<int>> sets;
    while(!_parsed["FIELD_NAME_EVAL"].empty()) {
        sets.push_back(generate_set(_parsed["FIELD_NAME_EVAL"][0],
                _parsed["VALUE"][0], _parsed["OPERATOR"][0]));
        _parsed["FIELD_NAME_EVAL"].erase(_parsed["FIELD_NAME_EVAL"].begin());
        _parsed["VALUE"].erase(_parsed["VALUE"].begin());
        _parsed["OPERATOR"].erase(_parsed["OPERATOR"].begin());
        if (!_parsed["LOGIC"].empty()) {
            while (!op_stk.empty() && _parsed["LOGIC"][0] == "OR" && op_stk.top() == "AND") {
                auto s1 = sets.back();
                sets.pop_back();
                auto s2 = sets.back();
                sets.pop_back();
                auto and_set = and_of_set(s1, s2);
                sets.push_back(and_set);
                op_stk.pop();
            }
            op_stk.push(_parsed["LOGIC"][0]);
            _parsed["LOGIC"].erase(_parsed["LOGIC"].begin());
        }

    }
    while (!op_stk.empty()) {
        if(op_stk.top() == "AND") {
            auto s1 = sets.back();
            sets.pop_back();
            auto s2 = sets.back();
            sets.pop_back();
            auto and_set = and_of_set(s1, s2);
            sets.push_back(and_set);
        }
        if(op_stk.top() == "OR") {
            auto s1 = sets.back();
            sets.pop_back();
            auto s2 = sets.back();
            sets.pop_back();
            auto and_set = or_of_set(s1, s2);
            sets.push_back(and_set);
        }
        op_stk.pop();
    }
    return sets[0];
}

void Table::print_fields() {
    for (auto && _field : _fields) {
        cout << _field << "; ";
    }
    cout << endl;
}


void Table::print_table() {
    for (auto & it : _table) {
//        it.print_list();
        it.print_graph();
        cout << endl;
    }
}

const size_t Table::get_field_count() const{
    return _fields.size();
}

vector<int> Table::generate_set(string field_name, string value, string op) {
    vector<int> result;
    auto &t = _table[_fields_map[field_name]];
    auto pivot = t.find(value);
    bool equal = true;
    if (pivot.is_null()) {
        pivot = t.find_ge(value);
        equal = false;
    }

    if (op == "=") {
        if(equal)
            result.insert(result.end(), pivot->value_list.begin(), pivot->value_list.end());
    }
    else if (op == "<") {
        for (auto it = t.begin(); it != pivot; ++it) {
            result.insert(result.end(), it->value_list.begin(), it->value_list.end());
        }
    }
    else if (op == "<=") {
        for (auto it = t.begin(); it != pivot; ++it) {
            result.insert(result.end(), it->value_list.begin(), it->value_list.end());
        }
        if(equal) {
            result.insert(result.end(), pivot->value_list.begin(), pivot->value_list.end());
        }
    }
    else if (op == ">") {
        auto temp = pivot;
        if(equal) {
            ++temp;
        }
        for (auto it = temp; it != t.end(); ++it) {
            result.insert(result.end(), it->value_list.begin(), it->value_list.end());
        }
    }
    else if (op == ">=") {
        for (auto it = pivot; it != t.end(); ++it) {
            result.insert(result.end(), it->value_list.begin(), it->value_list.end());
        }
    }
    return result;
}

vector<int> Table::and_of_set(vector<int> s1, vector<int> s2) {
    vector<int> inters_vector(s1.size()+s2.size());
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());
    auto it = set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), inters_vector.begin());
    inters_vector.resize(it - inters_vector.begin());
    return inters_vector;
}

vector<int> Table::or_of_set(vector<int> s1, vector<int> s2) {
    vector<int> union_vector(s1.size()+s2.size());
    // s1 and s2 need to be sorted!
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());
    auto it = set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), union_vector.begin());
    union_vector.resize(it - union_vector.begin());
    return union_vector;
}

//const vector<string> Table::get_fields_vector() const {
//    return _fields;
//}
