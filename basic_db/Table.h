/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#ifndef INC_0520_FINAL_TABLE_H
#define INC_0520_FINAL_TABLE_H

#include <vector>
#include "libs/Map.h"
#include "libs/MMap.h"

class Table {
public:
    Table();
    Table (string name);
    Table (string name, vector<string> fields);
    void insert_into(vector<string> values, size_t rec_no);
    vector<int> select_record_numbers(MMap<string, string> _parsed);

    void print_fields();
    void print_table();
    const size_t get_field_count() const;
//    const vector<string> get_fields_vector() const;
     vector<string> _fields;
    Map<string, int> _fields_map;
private:
    string _table_name;
    vector<MMap<string, int>> _table;
    vector<int> generate_set(string field_name, string value, string op);
    vector<int> and_of_set(vector<int> s1, vector<int> s2);
    vector<int> or_of_set(vector<int> s1, vector<int> s2);
};

#endif //INC_0520_FINAL_TABLE_H
