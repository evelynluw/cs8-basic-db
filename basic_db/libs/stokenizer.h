/*
 * Author: Evelyn Lu
 * Date: 3/4
 * Project: String Tokenizer
 * Purpose: Tokenize a string
 * File: stokenizer.h declarations for Token and STokenizer class
*/

#ifndef STOKENIZER_H
#define STOKENIZER_H
#include <iostream>
#include <iomanip>
#include <map>

using namespace std;

const map<int, string> types_map = //machine starting state and type_name string
        //can use enum too, with successor function?
        {{0, "NUMBER"},
         {4, "ALPHA"},
         {6, "SPACE"},
         {8, "PUNC"},
         {10, "OPERATOR"},
         {12, "QUOTED"}
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

class STokenizer
{
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
    string _buffer;
    static int _table[30][256];
    int _pos_current;
    int _pos_begin;
    int _pos_last_suc;
    int _state_last_suc;
    const int num_of_machines = 4;
    const int machines[4] = {0, 4, 6, 8}; //same as num_of_machines
    bool end; //true if at the end of string
    void init_table();
    void fill_table_double();
    void fill_table_word();
    void fill_table_space();
    void fill_table_punc();
    void fill_table_operator();
    void fill_table_quoted_word();
    void mark_success(int state);
    void mark_fail(int state);
    bool is_success(int state);
    void mark_cells(int row, int from, int to, int state);
    void mark_cells(int row, const char columns[], int state);
    void mark_cell(int row, int column, int state);
};

#endif // STOKENIZER_H
