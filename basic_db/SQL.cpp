/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#include "SQL.h"
#include <iomanip>

SQL::SQL(): recno(), cmdno() {

}

void SQL::run() {
    bool debug = false, run = true;
    fstream fstm, outf;
    remove("output.txt");
    Record::open_fileRW(outf, "output.txt");
    string filename;
    cout << "> ";
    cin >> filename;
    Record::open_fileRW(fstm, filename.c_str());
    while (run){
        get_input(run, debug, fstm, outf);
        process_commands(debug, outf);
    }
    outf.close();
    outf.clear();
    Record::open_fileRW(outf, "output.txt");
    Record::print_file(outf);


}

void SQL::get_input(bool& run, bool debug, istream& ins, ostream& outs) {
    string input;
    outs << "\n";
    getline(ins, input);
    if (ins.eof()) {
        run = false;
        _parsed.clear();
        return;
    }
    if (input.empty()) {
        _parsed.clear();
        return;
    }
    if (input[0] == '/') {
        outs << input;
        _parsed.clear();
        return;
    }
    Parser p(input);
    p.process();
    _parsed = p.get_parsed();
    outs << "[" << cmdno++ <<"] " << input << "\n";
    if (debug) {
        _parsed.print_list();
        outs << "\n";
    }
}

void SQL::process_commands(bool debug, ostream& outs) {
    /*
       * :create table students fields first, last, age, major
       * COMMAND: create,
       * FIELD_NAME: first, last, age, major,
       * TABLE_NAME: students,
       */
    if (_parsed.empty())
        return;
    string command = _parsed["COMMAND"][0];
    transform(command.begin(), command.end(), command.begin(), ::toupper);
    if (command == "CREATE" || command == "MAKE") {
        string name = _parsed["TABLE_NAME"][0];

        _tables.insert(name, Table(name, _parsed["FIELD_NAME"]));
        _records.insert(name, Record(_parsed["FIELD_NAME"].size(), name + ".rcd"));
        outs << "SQL: [" << name << "]" << " is created. \n";
        if (debug) {
            _tables[name].print_fields();
        }
//        _r.set_field_count(_parsed["FIELD_NAME"].size());
    }
        /*
         * :insert into students values Richard, Wang, 20, CS
         * COMMAND: insert,
         * TABLE_NAME: students,
         * VALUE: Richard, Wang, 20, CS,
         */
    else if (command == "INSERT") {
        if (!_tables.contains(_parsed["TABLE_NAME"][0])) {
            outs << "CAN'T INSERT INTO AN UNKNOWN TABLE\n";
        } else {
            string name = _parsed["TABLE_NAME"][0];
            // HERE A RECORD NEED TO BE CREATED / AND WRITE THIS RECORD INTO
            // A FILE;
            _records[name].set_record(_parsed["VALUE"]);
            recno = _records[name].save_list();
            _tables[_parsed["TABLE_NAME"][0]].insert_into(
                    _parsed["VALUE"], recno);
            outs << "SQL: inserted record into [" << name << "]\n";
            if (debug) {
                _tables[_parsed["TABLE_NAME"][0]].print_table();
            }
        }
    }

        /*
         * :select * from students
         *  COMMAND: select,  FIELD_NAME: *,  TABLE_NAME: students,
         * :select * from students where age = 20
         *  COMMAND: select,  FIELD_NAME: *,  FIELD_NAME_EVAL: age,  OPERATOR: =,
         *  TABLE_NAME: students,  VALUE: 20,
         * :select * from students where last < L
         *  COMMAND: select,  FIELD_NAME: *,  FIELD_NAME_EVAL: last,  OPERATOR: <,
         *  TABLE_NAME: students,  VALUE: L,
         * :select first from students where age > 18
         *  COMMAND: select,  FIELD_NAME: first,  FIELD_NAME_EVAL: age,
         *  OPERATOR: >,  TABLE_NAME: students,  VALUE: 18,
         * :select first from students where age > 18 and major = CS
         *  COMMAND: select,  FIELD_NAME: first,  FIELD_NAME_EVAL: age, major,
         *  LOGIC: and,  OPERATOR: >, =,  TABLE_NAME: students, VALUE: 18, CS,
         */

    else if (command == "SELECT") {
        if (!_tables.contains(_parsed["TABLE_NAME"][0])) {
            outs << "CAN'T SELECT FROM AN UNKNOWN TABLE\n";
        } else {
            string name = _parsed["TABLE_NAME"][0];
            auto t = &(_tables[name]);
            vector<int> field_number_list;

            // parse the fields to output &
            //  print the "header" of the table

            if (_parsed["FIELD_NAME"][0] == "*") {
                int s = t->_fields.size();
                for (size_t j = 0; j < s; ++j) {
                    field_number_list.push_back(j);
                }
            } else {
                for(auto &field : _parsed["FIELD_NAME"]) {
                    field_number_list.push_back(t->_fields_map[field]);
                }
            }
            int max = _r.MAX;
            outs << setw(5) << "#";
            for (auto field_num = field_number_list.begin();
                 field_num != field_number_list.end(); ++field_num) {
                outs << setw(max) << t->_fields[*field_num];
            }
            outs << '\n';
            size_t length = 5 + _r.MAX * field_number_list.size();
            for (size_t i = 0; i < length; ++i) {
                outs << '-';
            }
            outs << '\n';

            if (!_parsed.contains("FIELD_NAME_EVAL")) {
                // no criteria for records; select all items from the table.
                //  using the field name filter

                _records[name].output_records(field_number_list, {-1}, outs);
            } else {
                // give the parsed map to the table class to handle with
                //  the shunting yard algorithm.
                auto recno_list = t->select_record_numbers(_parsed);
//                if(debug) {
//                    outs << recno_list << endl;
//                }
                _records[name].output_records(field_number_list, recno_list, outs);
            }

        }

    }
}
