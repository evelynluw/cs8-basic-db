#include <iostream>
//#include "Parser.h"
#include "SQL.h"

using namespace std;

void test_parser();

int main() {
    SQL sql;
    sql.run();
    return 0;
}

void test_parser() {
    Parser parser;
    parser.set_string("create table students fields first, last, age, major");
    parser.process();
    auto parsed = parser.get_parsed();
    parsed.print_list();
    parser.set_string("insert into students values Richard, Wang, 20, CS, student");
    parser.process();
    parser.get_parsed().print_list();
    parser.set_string("select * from students where first = Jonas and last = Bill");
    parser.process();
    parser.get_parsed().print_list();

}
