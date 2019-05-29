/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#ifndef INC_0520_FINAL_CMDTOKENIZER_H
#define INC_0520_FINAL_CMDTOKENIZER_H

#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "libs/Map.h"

const map<int, string> command_types = //machine starting state and type_name string
        //can use enum too, with successor function?
        {{0, "NUMBER"},
         {4, "ALPHA"},
         {6, "SPACE"},
         {8, "PUNC"}
        };

const map<string, int> columns = {
        {"CREATE", 1},
        {"INSERT", 2},
        {"SELECT", 3},
        {"OPERATORS", 4},
        {"LOGIC", 5},
        {"TABLE", 6},
        {"<DISCARDED>", 7},
        {"<TABLE_NAME>", 8},
        {"<FIELD_NAME>", 9},
        {"VALUE", 10}
};

class Token
{
public:
    //CTOR
    Token(): _type_num(), _content() {}
    Token(int type_num, string content): _type_num(type_num),
                                         _content(content) {}
    //Accessors
    friend ostream& operator<<(ostream& outs, Token& t);
    int type();
    string type_string();
    string content();
    void set_type(int type_num);
    void set_content(string content_str);

private:
    int _type_num;
    string _content;
};

class STokenizer {
public:
    //CTOR
    STokenizer();
    STokenizer(string buffer);
    void make_table(); //fill out the table with machines
    //ACCESS
    int get_current_pos();
    bool done();
    //MODIFIER
    void set_string(string str);
    //PROCESS
    bool get_token(int starting_state);
    string get_current_token();
    //OUTPUT
    friend STokenizer& operator>>(STokenizer &stk, Token &t);

protected:
    vector<string> _buffer;
    static int _table[30][20];
    int _pos_current;
    int _pos_begin;
    int _pos_last_suc;
    int _state_last_suc;
    const int num_of_machines = 4;
    const int machines[4] = {0, 4, 6, 8}; //same as num_of_machines
    bool end; //true if at the end of string
    void init_table();
    void fill_table_double();
//    void fill_table_word();
//    void fill_table_space();
//    void fill_table_punc();
    void fill_table_commands();
    void mark_success(int state);
    void mark_fail(int state);
    bool is_success(int state);
    void mark_cells(int row, int from, int to, int state);
    void mark_cells(int row, const char columns[], int state);
    void mark_cell(int row, int column, int state);
    int find_column(string word);
};


#endif //INC_0520_FINAL_CMDTOKENIZER_H
