/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/
#include <cstring>
#include <iomanip>
#include "Record.h"

Record::Record(string filename): recno(-1), field_count(0) {
    clear_record();
    record_file = filename;
    remove(record_file.c_str());
    record[0][0] = NULL;
}

Record::Record(int _field_count, string filename): recno(-1), field_count(_field_count) {
    clear_record();
    record_file = filename;
    remove(record_file.c_str());
}

Record::Record(vector<string> value_list, string filename) {
    clear_record();
    record_file = filename;
    remove(record_file.c_str());
    set_record(value_list);
}

void Record::clear_record() {
    for (size_t i = 0; i < MAX; ++i) {
        for (size_t j = 0; j < MAX; ++j) {
            record[i][j] = '\0';
        }
    }
}

void Record::set_record(vector<string> value_list) {
    clear_record();
    for (size_t i = 0; i < field_count; ++i) {
        strcpy(record[i], value_list[i].c_str());
    }
    field_count = value_list.size();
}

void Record::set_field_count(int _field_count) {
    field_count = _field_count;
}

void Record::set_filename(string filename) {
    record_file = filename;
}

//! write current record to an already opened stream.
//! \param outs output filestream
//! \return recno or record number
long Record::write(fstream &outs) {
    for (size_t i = 0; i < field_count; ++i) {
        outs.write(record[i], sizeof(char[MAX]));
    }
//    long pos = outs.tellp();
    return ++recno;
}

//! read the record with recno to the record 2d array.
//! \param ins input filestream
//! \param _recno # of the record to be read
//! \return number of characters extracted by reading
long Record::read(fstream &ins, long _recno) {
    recno = _recno;
    long pos = _recno*sizeof(char[MAX])*field_count;
    ins.seekg(pos, ios_base::beg);
    for (size_t i = 0; i < field_count; ++i) {
        ins.read(record[i], sizeof(char[MAX]));
    }
    return ins.gcount();
}

ostream &operator<<(ostream &outs,
                    const Record &r) {
    for (size_t i = 0; i < r.field_count; ++i) {
        outs << r.record[i] << ", ";
    }
    return outs;
}

//-------------------------------------------------
bool Record::file_exists(const char filename[]) {
    const bool debug = false;
    fstream ff;
    ff.open(filename,
            std::fstream::in | std::fstream::binary);
    if (ff.fail()) {
        if (debug) cout << "file_exists(): File does NOT exist: " << filename << endl;
        return false;
    }
    if (debug) cout << "file_exists(): File DOES exist: " << filename << endl;
    ff.close();
    return true;
}

void Record::open_fileRW(fstream &f, const char filename[]) throw(char*) {
    const bool debug = false;
    //opening a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)) {
        f.open(filename, std::fstream::out | std::fstream::binary);
        if (f.fail()) {
            cout << "file open (RW) failed: with out|" << filename << "]" << endl;
            throw ("file RW failed  ");
        } else {
            if (debug) cout << "open_fileRW: file created successfully: " << filename << endl;
        }
    } else {
        f.open(filename,
               std::fstream::in | std::fstream::out | std::fstream::binary);
        if (f.fail()) {
            cout << "file open (RW) failed. [" << filename << "]" << endl;
            throw ("file failed to open.");
        }
    }

}

void Record::open_fileW(fstream &f, const char filename[]) {
    f.open(filename, std::fstream::out | std::fstream::binary | std::fstream::app);
//    f.open(filename, std::fstream::out | std::fstream::binary);
    if (f.fail()) {
        cout << "file open failed: " << filename << endl;
        throw ("file failed to open.");
    }

}

void Record::print_file(fstream &f) {
//    cout.flush();
//    cout << flush;
    string line;
    getline(f, line);
    while(!f.eof()) {
        cout << line << endl;
        getline(f, line);
    }
}

long Record::save_list() {
    long temp;
    fstream f;
    open_fileW(f, record_file.c_str());
    temp = this->write(f);
    f.close();
    return temp;
}

void Record::output_records(const vector<int> fields_number_list,
        const vector<int> record_number_list, ostream &outs) {
    long temp = recno;
    fstream f;
//    Record r(field_count);
    open_fileRW(f, record_file.c_str());
    if (!record_number_list.empty() && record_number_list[0] == -1) { // print all records
        for (size_t i = 0; i <= temp; ++i) {
            read(f, i);
            outs << setw(5) << i;
            for(int it : fields_number_list) {
                outs << setw(MAX) << record[it] ;
            }
            outs << endl;
        }
    }
    else { // print only records from the record number list
        for(int i : record_number_list) {
            read(f, i);
            outs << setw(5) << i;
            for(int it : fields_number_list) {
                outs << setw(MAX) << record[it];
            }
            outs << endl;
        }
    }
    recno = temp;
}

void test_record_ios() {
    cout << endl << endl << endl << "================================" << endl;
    vector<string> vl0, vl1;
    vl0.push_back("zero");
    vl0.push_back("one");
    vl0.push_back("two");
    vl0.push_back("three");

    vl1.push_back("apple");
    vl1.push_back("banana");
    vl1.push_back("orange");
    vl1.push_back("mango");
    Record list(vl0);
    cout << list << endl;
    list.save_list();
    list.set_record(vl1);
    list.save_list();

    Record r(4);
    fstream f;
    Record::open_fileRW(f, "record_list.bin");
    r.read(f, 0);
    cout << "record 0: " << r << endl;
    r.read(f, 1);
    cout << "record 1: " << r << endl;

    cout << endl << endl << endl << "================================" << endl;
}

