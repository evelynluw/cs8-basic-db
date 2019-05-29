/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#include "Parser.h"

void Parser::set_string(string str) {
    _str = str;
    parsed.clear();
}

/*
 * GRAMMAR:
 * <CREATE | MAKE> : {  <create | make> table TABLE_NAME fields FIELD_NAME [, FIELD_NAME...]  }
<INSERT> : { insert into TABLE_NAME values VALUE [, VALUE...]      }
<SELECT> : { select <* | FIELD_NAME> [, ,FIELD_NAME>...]
					from TABLE_NAME
					where FIELD_NAME <RELATIONAL_OPERATOR> VALUE
						[<LOGICAL_OPERATOR>
							FIELD_NAME <RELATIONAL_OPERATOR> VALUE...]
			}

VALUE  : A string of alphanumeric characters, or a string of alphanumeric
 			characters and spaces enclosed by double quotation marks:
 			"Jean Luise", Finch, 1923
<RELATIONAL OPERATOR> : [ = | > | < | >= | <= ]
<LOGICAL OPERATOR>    : [and | or]
 */

/*! here breaks down the parsing process.
 * First get the word and operator tokens from the stk;
 * the from this vector, using a switch to see what should be inserted to the
 * map of parsed commands/arguments.
 * Note: in case of the "symbol", we need to check the previous state to see
 * which case should be true. This requires the table built in the columns for
 * the TABLE_NAME, FIELD NAME, AND VALUE, etc.
 */
void Parser::process() {
    parsed.clear();
    STokenizer stk;
    vector<Token> token_v;
    int cur_c = 0, prev_c = 0, prev_state = 0, cur_state = 0;
    bool discard = false;
    stk.set_string(_str);
    Token stk_token;
    while(!stk.done()) {
        stk >> stk_token;
        string stk_token_typestr = stk_token.type_string();
        if(stk_token_typestr == "ALPHA" || stk_token_typestr == "NUMBER"
            ||stk_token_typestr == "OPERATOR") {
            token_v.push_back(stk_token);
        }
    }
    for (auto it = token_v.begin();
    it != token_v.end(); ++it) {
        cur_c = find_column(it->content());
        if (cur_c == columns.at("SYMBOL")) {
            // this means find_column couldn't tell what type of string the
            // current word is; we have to check using the state machine.

            // is this alright? because we are not going to the last state
            // is pointing us to. we're going to wherever column with a number
            // that is not -1. this means when we have multiple meanings for
            // symbol, we're having a problem.
            // BUT there is a sequence to these searching.
            if(table[prev_state][columns.at("DISCARD")] != -1) {
                cur_c = columns.at("DISCARD");
                discard = true;
            }
            else if(table[prev_state][columns.at("TABLE_NAME")] != -1) {
                cur_c = columns.at("TABLE_NAME");
            }
            else if(table[prev_state][columns.at("FIELD_NAME")] != -1) {
                cur_c = columns.at("FIELD_NAME");
            }
            else if(table[prev_state][columns.at("VALUE")] != -1) {
                cur_c = columns.at("VALUE");
            }
            else if(table[prev_state][columns.at("OPERATORS")] != -1) {
                cur_c = columns.at("OPERATORS");
            }
            else if(table[prev_state][columns.at("LOGIC")] != -1) {
                cur_c = columns.at("LOGIC");
            }
        }

        cur_state = table[prev_state][cur_c];
        if(cur_state != -1 && !discard) { // this means the current state is legal
            string type = type_parsed.at(cur_state);
            parsed.insert(type, it->content());
        } else { // current state is not legal; clear the parsed map and
            // ? return false?

        }

        if(! table[cur_state][0]) { // if the end state is not success
            // return false and clear the parsed map
        }
        prev_c = cur_c;
        prev_state = cur_state;
        discard = false;

    }
    parsed.erase("DISCARD");

}

MMap<string, string> Parser::get_parsed() {
    return parsed;
}

void Parser::fill_table() {
    init_table();
    mark_cell(0, columns.at("CREATE"), 1);
    mark_cell(1, columns.at("DISCARD"), 2);
    mark_cell(2, columns.at("TABLE_NAME"), 3);
    mark_cell(3, columns.at("DISCARD"), 4);
    mark_cell(4, columns.at("FIELD_NAME"), 5);
    mark_cell(5, columns.at("FIELD_NAME"), 5);

    mark_cell(0, columns.at("INSERT"), 6);
    mark_cell(6, columns.at("DISCARD"), 7);
    mark_cell(7, columns.at("TABLE_NAME"), 8);
    mark_cell(8, columns.at("DISCARD"), 9);
    mark_cell(9, columns.at("VALUE"), 10);
    mark_cell(10, columns.at("FIELD_NAME"), 10);
    mark_success(10);

    mark_cell(0, columns.at("SELECT"), 11);
    mark_cell(11, columns.at("FIELD_NAME"), 12);
    mark_cell(12, columns.at("FIELD_NAME"), 12);
    mark_cell(12, columns.at("FROM"), 13);
    mark_cell(13, columns.at("TABLE_NAME"), 14);
    mark_cell(14, columns.at("DISCARD"), 15);
    mark_cell(15, columns.at("FIELD_NAME"), 16);
    mark_cell(16, columns.at("OPERATORS"), 17);
    mark_cell(17, columns.at("VALUE"), 18);
    mark_cell(18, columns.at("LOGIC"), 19);
    mark_cell(19, columns.at("FIELD_NAME"), 16);
    mark_success(14);
    mark_success(18);

}

void Parser::init_table() {
    for (int r = 0; r < 30; ++r) {
        table[r][0] = 0;
        for(int c = 1; c < 30; ++c)
            table[r][c] = -1;
    }
}

//Mark this state (row) with a 1 (success)
void Parser::mark_success(int state) {
    table[state][0] = 1; //mark the states that returns successful tokens as 1
}
//Mark this state (row) with a 0 (fail)
void Parser::mark_fail(int state) {
    table[state][0] = 0; //mark the states that doesnt return successful tokens
}
//true if state is a success state
bool Parser::is_success(int state) {
    return table[state][0];
}
//Mark a range of cells in the array.
void Parser::mark_cells(int row, int from, int to, int state) {
    for(int c = from; c <= to; ++c)
        table[row][c] = state;
}
//Mark columns represented by the string columns[] for this row
void Parser::mark_cells(int row, const char columns[], int state) {
    int i = 0;
    while(columns[i] != '\0') {
        table[row][columns[i]] = state;
        ++i;
    }

}
//Mark this row and column
void Parser::mark_cell(int row, int column, int state) {
    table[row][column] = state;
}

int Parser::find_column(string word) {
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    auto c = columns.find(word);
    if (c == columns.end()) {
        return columns.at("SYMBOL");
    }
    return c->second;
}

