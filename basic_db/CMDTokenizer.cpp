/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#include "CMDTokenizer.h"


int STokenizer::_table[30][20]; //since table is static member

void Token::set_type(int type_num) {
    _type_num = type_num;
}

void Token::set_content(string content_str) {
    _content = content_str;
}

ostream& operator<<(ostream& outs, Token& t) {
    outs<<setw(10)<<t.type_string();
    outs<<setw(10)<<'|'<<t._content<<'|'<<endl;
    return outs;
}

int Token::type() {
    return _type_num;
}

string Token::type_string() {
    return command_types.at(_type_num); //cannot use [] for const map
}

string Token::content() {
    return _content;
}

STokenizer::STokenizer(): _buffer(), _pos_current(),
                          _pos_last_suc(), _state_last_suc(), end(false) {
    make_table();
}

STokenizer::STokenizer(string buffer): _pos_current(), _pos_last_suc(),
                                       _state_last_suc(), end(false) {
    set_string(buffer);
    make_table();
}

void STokenizer::make_table() {
    init_table();
    fill_table_commands();

}

int STokenizer::get_current_pos() {
    return _pos_current;
}

bool STokenizer::done() {
    return end;
}

void STokenizer::set_string(string str) {
    _buffer = str;
    end = false;
}

bool STokenizer::get_token(int starting_state) {
    //runs only a single machine and stops the loop
    //  when reaching fail or end of string
    //state: the starting state of a machine, number given
    //  by another function
    //other given: (mem vars) (string)buffer and (int) pos_current
    int state = starting_state;
    _pos_begin = _pos_current;
    while ((state = _table[state][find_column(_buffer[_pos_current])]) != -1
           && _pos_current!= _buffer.size()) { //not fail, not EOS
        if(_table[state][0] == 1) {//if current state is success
            _pos_last_suc = _pos_current;
            _state_last_suc = state;
        }
        ++_pos_current; //points to next char to check
    };
    if(_pos_current == _buffer.size())
        end = true;
    if(_pos_current == _pos_begin)
        return false;
    if(_pos_last_suc < _pos_begin){
        _pos_current=_pos_begin;
        return false;
    }
    _pos_current = _pos_last_suc+1;
    return true;
    //_pos_current modified
}

string STokenizer::get_current_token() {
    return _buffer.substr(_pos_begin, _pos_current-_pos_begin);
}

STokenizer& operator>>(STokenizer &stk, Token &t) {
    //extract only one token; but loops through all different machines.
    //starting from _current_pos. get the next token.
    //modifies (Token)t, the token just extracted.
    //This Runs
    map<int,string>::const_iterator it;
    int state;
    for(it = command_types.begin(); it != command_types.end(); ++it) {
        state = it->first;
        if(stk.get_token(state)) {
            t.set_type(state);
            t.set_content(stk.get_current_token());
            return stk;
        }
    }
    return stk;
}

void STokenizer::init_table() {
    for(int r = 0; r < 30; ++r) {
        _table[r][0] = 0;
        for(int c = 1; c < 256; ++c)
            _table[r][c] = -1;
    }
}

void STokenizer::fill_table_double() {
    //begins with state 0
    mark_cells(0, '0', '9', 1);
    mark_cell(0, '.', 2);
    mark_cells(1, '0', '9', 1);
    mark_success(1);
    mark_cell(1, '.', 2);
    mark_cells(2, '0', '9', 3);
    mark_cells(3, '0', '9', 3);
    mark_success(3);
}

/*
void STokenizer::fill_table_word() {
    //begins with state 4
    mark_cells(4,'A', 'Z', 5);
    mark_cells(4, 'a', 'z', 5);
    mark_cells(5,'A', 'Z', 5);
    mark_cells(5, 'a', 'z', 5);
    mark_success(5);
}

void STokenizer::fill_table_space() {
    mark_cell(6, ' ', 7);
    mark_cell(7, ' ', 7);
    mark_success(7);
}

void STokenizer::fill_table_punc() { //CHANGE THIS
    //begins with state 6
    mark_cells(8, ",.?!:;\"'", 9);
    mark_cells(9, ",.?!:;\"'", 9);
    mark_success(9);
}
 */

void STokenizer::fill_table_commands() {
    //begins with state 0
    mark_cell(0, columns.at("CREATE"), 1);
    mark_cell(1, columns.at("<DISCARDED>"), 2);
    mark_cell(2, columns.at("<TABLE_NAME>"), 3);
    mark_cell(3, columns.at("<DISCARDED>"), 4);
    mark_cell(4, columns.at("<FIELD_NAME>"), 4);
    mark_success(4);

//    mark_cell(0, columns.at("INSERT"), 5); //tbd
//    mark_cell(0, columns.at("INSERT"), 5);
//    mark_cell(0, columns.at("INSERT"), 5);
//    mark_cell(0, columns.at("INSERT"), 5);
//    mark_cell(0, columns.at("INSERT"), 5);
//
//    mark_cell(0, columns.at("SELECT"), 20); //tbd


}

//Mark this state (row) with a 1 (success)
void STokenizer::mark_success(int state) {
    _table[state][0] = 1; //mark the states that returns successful tokens as 1
}
//Mark this state (row) with a 0 (fail)
void STokenizer::mark_fail(int state) {
    _table[state][0] = 0; //mark the states that doesnt return successful tokens
}
//true if state is a success state
bool STokenizer::is_success(int state) {
    return _table[state][0];
}
//Mark a range of cells in the array.
void STokenizer::mark_cells(int row, int from, int to, int state) {
    for(int c = from; c <= to; ++c)
        _table[row][c] = state;
}
//Mark columns represented by the string columns[] for this row
void STokenizer::mark_cells(int row, const char columns[], int state) {
    int i = 0;
    while(columns[i] != '\0') {
        _table[row][columns[i]] = state;
        ++i;
    }

}
//Mark this row and column
void STokenizer::mark_cell(int row, int column, int state) {
    _table[row][column] = state;
}

int STokenizer::find_column(string word) {
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    // can't use a switch here!
    if (word == "CREATE" || word == "MAKE")
        return 1;
    if (word == "INSERT")
        return 2;
    if (word == "SELECT")
        return 3;
    if (word == "+" || word == "-")
        return 4;
    if (word == "LOGIC")
        return 5;
    if (word == "TABLE" || word == "FIELDS")
        return 6;
    return 19; // reserved column now
}
