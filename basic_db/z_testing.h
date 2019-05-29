/*
 * Author: Evelyn Lu
 * Project: 
 * Purpose: 
 * File: 
*/

#ifndef INC_0520_FINAL_Z_TESTING_H
#define INC_0520_FINAL_Z_TESTING_H

/*
"C:\Users\Evelyn\Google Drive (ylu40@go.pasadena.edu)\CS 8\Projects\0520_Final\0520_Final\cmake-build-debug\0520_Final.exe"
:create table students fields first, last, age, major
COMMAND: create,  FIELD_NAME: first, last, age, major,  TABLE_NAME: students,
[students] is created.
first; last; age; major;
:insert into students values Richard, Wang, 20, CS
COMMAND: insert,  TABLE_NAME: students,  VALUE: Richard, Wang, 20, CS,
inserted values into [students, ]
Richard: 0,
Wang: 0,
20: 0,
CS: 0,
:insert into students values Jack, Zhao, 23, CS
COMMAND: insert,  TABLE_NAME: students,  VALUE: Jack, Zhao, 23, CS,
inserted values into [students, ]
Jack: 1,  Richard: 0,
Wang: 0,  Zhao: 1,
20: 0,  23: 1,
CS: 0, 1,
:insert into students values Lydia, Smith, 20, Physics
COMMAND: insert,  TABLE_NAME: students,  VALUE: Lydia, Smith, 20, Physics,
inserted values into [students, ]
Jack: 1,  Lydia: 2,  Richard: 0,
Smith: 2,  Wang: 0,  Zhao: 1,
20: 0, 2,  23: 1,
CS: 0, 1,  Physics: 2,
:insert into students values Joe, Blow, 23, Math
COMMAND: insert,  TABLE_NAME: students,  VALUE: Joe, Blow, 23, Math,
inserted values into [students, ]
Jack: 1,  Joe: 3,  Lydia: 2,  Richard: 0,
Blow: 3,  Smith: 2,  Wang: 0,  Zhao: 1,
20: 0, 2,  23: 1, 3,
CS: 0, 1,  Math: 3,  Physics: 2,
:insert into students values Ricky, Martin, 40, Music
COMMAND: insert,  TABLE_NAME: students,  VALUE: Ricky, Martin, 40, Music,
inserted values into [students, ]
Jack: 1,  Joe: 3,  Lydia: 2,  Richard: 0,  Ricky: 4,
Blow: 3,  Martin: 4,  Smith: 2,  Wang: 0,  Zhao: 1,
20: 0, 2,  23: 1, 3,  40: 4,
CS: 0, 1,  Math: 3,  Music: 4,  Physics: 2,

Process finished with exit code 1

 * */

#endif //INC_0520_FINAL_Z_TESTING_H
