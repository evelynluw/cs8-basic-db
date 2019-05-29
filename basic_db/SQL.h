/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#ifndef INC_0520_FINAL_SQL_H
#define INC_0520_FINAL_SQL_H

#include "Parser.h"
#include "Table.h"
#include "Record.h"
#include "libs/Map.h"
#include "libs/MMap.h"
#include <stack>



class SQL {
public:
    // set up your _tables mmap;
    SQL();
    void run();
    void get_input(bool& run, bool debug, istream& ins = cin, ostream& outs = cout);
    void process_commands(bool debug = true, ostream& outs = cout);
private:
    //! for each field we have a single table.
    //! Inside each table we store all the index (recno)
    //! of field value of the corresponding type.
    Map<string, Table> _tables;
    MMap<string, string> _parsed;
    Map<string, Record> _records;
    Record _r;
    long recno;
    int cmdno;
};



#endif //INC_0520_FINAL_SQL_H
