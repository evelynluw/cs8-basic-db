/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: reading and writing records
 * File: 
*/

#ifndef INC_0520_FINAL_RECORD_H
#define INC_0520_FINAL_RECORD_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Record {
public:
    Record(string filename = "record_list.bin");
    Record(int _field_count, string filename = "record_list.bin");
    Record(vector<string> value_list, string filename = "record_list.bin");
    void set_record(vector<string> value_list);
    void set_field_count(int _field_count);
    void set_filename(string filename);
    long write(fstream &outs);
    long read(fstream &ins, long _recno);
    long save_list();
    void output_records(const vector<int> fields_number_list, const vector<int> record_number_list, ostream &outs = cout);
    friend ostream &operator<<(ostream &outs,
                               const Record &r);
    static bool file_exists(const char filename[]);
    static void open_fileRW(fstream &f, const char filename[]) throw(char*);
    static void open_fileW(fstream &f, const char filename[]);
    static void print_file(fstream &f);

    int field_count;
    long recno;
    static const int MAX = 15;
private:
    char record[MAX][MAX];
//    inline static const string record_file = "record_list.bin";
    string record_file;
    void clear_record();
};

// other functions

void test_record_ios();

#endif //INC_0520_FINAL_RECORD_H
