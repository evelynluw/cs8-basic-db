/*
 * Author: Evelyn Lu
 * Project: Final: Database
 * Purpose: Access a table with command line arguments
 * File: Parser.h definitions for the parser class.
*/

#ifndef INC_0520_FINAL_PARSER_H
#define INC_0520_FINAL_PARSER_H

#include <iostream>
#include "libs/Map.h"
#include "libs/MMap.h"
#include "libs/stokenizer.h"
#include <bits/stdc++.h>

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

class Parser {
public:
    Parser(): _str(), table() {
        fill_table();
    }
    Parser(const string &s): _str(s), table() {
        fill_table();
    }
    void set_string(string str);
    void process();
    MMap<string, string> get_parsed();
private:
    string _str;
    int table[30][30];
    MMap<string, string> parsed;
//    STokenizer stk;
//    vector<Token> token_v;
    Map<string, int> cmd_map;
    const map<string, int> columns = {
            {"CREATE", 1},
            {"MAKE", 1},
            {"INSERT", 2},
            {"SELECT", 3},
            {"OPERATORS", 4},
            {"LOGIC", 5},
            {"DISCARD", 6},
            {"TABLE_NAME", 7},
            {"FIELD_NAME", 8},
            {"VALUE", 9},
            {"FROM", 10},
            {"SYMBOL", 20}
    };
    const map<int, string> type_parsed = {
            {1, "COMMAND"},
            {2, "DISCARD"},
            {3, "TABLE_NAME"},
            {4, "DISCARD"},
            {5, "FIELD_NAME"},
            {6, "COMMAND"},
            {7, "DISCARD"},
            {8, "TABLE_NAME"},
            {9, "DISCARD"},
            {10, "VALUE"},
            {11, "COMMAND"},
            {12, "FIELD_NAME"},
            {13, "DISCARD"},
            {14, "TABLE_NAME"},
            {15, "DISCARD"},
            {16, "FIELD_NAME_EVAL"},
            {17, "OPERATOR"},
            {18, "VALUE"},
            {19, "LOGIC"}
    };
    enum col {
            CREATE = 1,
            INSERT = 2,
            SELECT = 3,
            OPERATORS = 4,
            LOGIC = 5,
            DISCARD = 6,
            SYMBOL = 20
    };

    /*
   * token_table["create'] += 1;
   * for ( ... 5)
   *      token_table["create"] += -1;
   * and so on...
  * */
    //! fill the table with commands according to the grammar at the top
    //! consult the excel file
    void fill_table();


    // SMALL TABLE FUNCTIONS
    void init_table();
    void mark_success(int state);
    void mark_fail(int state);
    bool is_success(int state);
    void mark_cells(int row, int from, int to, int state);
    void mark_cells(int row, const char columns[], int state);
    void mark_cell(int row, int column, int state);
    int find_column(string word);

};

#endif //INC_0520_FINAL_PARSER_H
