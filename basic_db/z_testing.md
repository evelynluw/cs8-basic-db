## Testing Results

### Sample (Employee and Student)

```
>_!sample.txt

//****************************************************************************

//              CREATE AND INSERT

//****************************************************************************


//---- employee table ----------

[0] make table employee fields  last,           first,                  dep
SQL: [employee] is created.

[1] insert into employee values Blow,           Joe,                    CS
SQL: inserted record into [employee]

[2] insert into employee values Johnson,        "Jimmy",                Chemistry
SQL: inserted record into [employee]

[3] insert into employee values Yang,           Bo,                     CS
SQL: inserted record into [employee]

[4] insert into employee values "Jackson",      Billy,                  Math
SQL: inserted record into [employee]

[5] insert into employee values Johnson,        Billy,                  "Phys Ed"
SQL: inserted record into [employee]

[6] insert into employee values "Van Gogh",     "Jim Bob",              "Phys Ed"
SQL: inserted record into [employee]


[7] select * from employee
    #           last          first            dep
--------------------------------------------------
    0           Blow            Joe             CS
    1        Johnson          Jimmy      Chemistry
    2           Yang             Bo             CS
    3        Jackson          Billy           Math
    4        Johnson          Billy        Phys Ed
    5       Van Gogh        Jim Bob        Phys Ed


//----- student table ----------

[8] make table student fields   fname,                  lname,          major,                          age
SQL: [student] is created.

[9] insert into student values  Flo,                    Yao,            CS,                             20
SQL: inserted record into [student]

[10] insert into student values         "Flo",                  "Jackson",      Math,                           21
SQL: inserted record into [student]

[11] insert into student values         Calvin,                 Woo,            Physics,                        22
SQL: inserted record into [student]

[12] insert into student values         "Anna Grace",   "Del Rio",      CS,                                     22
SQL: inserted record into [student]

[13] select * from student
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20
    1            Flo        Jackson           Math             21
    2         Calvin            Woo        Physics             22
    3     Anna Grace        Del Rio             CS             22




//****************************************************************************

//              SIMPLE SELECT

//****************************************************************************


[14] select * from student
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20
    1            Flo        Jackson           Math             21
    2         Calvin            Woo        Physics             22
    3     Anna Grace        Del Rio             CS             22


//------- simple strings -------------------

[15] select * from student where lname = Jackson
    #          fname          lname          major            age
-----------------------------------------------------------------
    1            Flo        Jackson           Math             21


//----- quoted strings ---------------------

[16] select * from student where lname = "Del Rio"
    #          fname          lname          major            age
-----------------------------------------------------------------
    3     Anna Grace        Del Rio             CS             22


//-------- non-existing string ------------------

[17] select * from student where lname = "Does Not Exist"
    #          fname          lname          major            age
-----------------------------------------------------------------


//****************************************************************************

//              RELATIONAL OPERATORS:

//****************************************************************************


//.................

//:Greater Than   :

//.................

[18] select * from student where lname > Yang
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20



//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .

[19] select * from student where age > 50
    #          fname          lname          major            age
-----------------------------------------------------------------


//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .

[20] select * from student where age  > 53
    #          fname          lname          major            age
-----------------------------------------------------------------


//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . .

[21] select * from student where age > 54
    #          fname          lname          major            age
-----------------------------------------------------------------


//.................

//:Greater Equal  :

//.................

[22] select * from student where lname >= Yang
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20

//. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . .

[23] select * from employee where last >= Jack
    #           last          first            dep
--------------------------------------------------
    3        Jackson          Billy           Math
    1        Johnson          Jimmy      Chemistry
    4        Johnson          Billy        Phys Ed
    5       Van Gogh        Jim Bob        Phys Ed
    2           Yang             Bo             CS



//.................

//:Less Than      :

//.................



//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . .

[24] select * from student where lname < Jackson
    #          fname          lname          major            age
-----------------------------------------------------------------
    3     Anna Grace        Del Rio             CS             22


//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .

[25] select * from student where age  < 20
    #          fname          lname          major            age
-----------------------------------------------------------------



//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . .


[26] select * from student where age  < 19
    #          fname          lname          major            age
-----------------------------------------------------------------



//.................

//:Less Equal     :

//.................


[27] select * from student where lname <= Smith
    #          fname          lname          major            age
-----------------------------------------------------------------
    3     Anna Grace        Del Rio             CS             22
    1            Flo        Jackson           Math             21


//. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . .

[28] select * from employee where last <= Peach
    #           last          first            dep
--------------------------------------------------
    0           Blow            Joe             CS
    3        Jackson          Billy           Math
    1        Johnson          Jimmy      Chemistry
    4        Johnson          Billy        Phys Ed


//****************************************************************************

//              LOGICAL OPERATORS

//****************************************************************************



//.................

//:AND            :

//.................


[29] select * from student where fname = "Flo" and lname = "Yao"
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20



//.................

//:OR            :

//.................

[30] select * from student where fname = Flo or lname = Jackson
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20
    1            Flo        Jackson           Math             21



//.................

//:OR AND         :

//.................

[31] select * from student where fname = Flo or major = CS and age <= 23
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20
    1            Flo        Jackson           Math             21
    3     Anna Grace        Del Rio             CS             22



//.................

//:AND OR AND     :

//.................


[32] select * from student where age <30 and major=CS or major = Physics and lname = Jackson
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20
    3     Anna Grace        Del Rio             CS             22



//.................

//:OR AND OR      :

//.................


[33] select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson
    #          fname          lname          major            age
-----------------------------------------------------------------
    0            Flo            Yao             CS             20
    1            Flo        Jackson           Math             21
    3     Anna Grace        Del Rio             CS             22

Process finished with exit code 0
```

### Student list

``` 
:create table students fields first, last, age, major
SQL: [students] is created.

:insert into students values Lydia, Smith, 19, Physics
SQL: inserted record into [students]

:insert into students values Joe, Blow, 23, CS
SQL: inserted record into [students]

:insert into students values Richard, Wang, 20, CS
SQL: inserted record into [students]

:insert into students values Ricky, Martin, 47, Music
SQL: inserted record into [students]

:insert into students values James, Lee, 24, Math
SQL: inserted record into [students]

:select * from students
    #          first           last            age          major
-----------------------------------------------------------------
    0          Lydia          Smith             19        Physics
    1            Joe           Blow             23             CS
    2        Richard           Wang             20             CS
    3          Ricky         Martin             47          Music
    4          James            Lee             24           Math

:select first, major from students
    #          first          major
-----------------------------------
    0          Lydia        Physics
    1            Joe             CS
    2        Richard             CS
    3          Ricky          Music
    4          James           Math

:select * from students where age >= 20
    #          first           last            age          major
-----------------------------------------------------------------
    2        Richard           Wang             20             CS
    1            Joe           Blow             23             CS
    4          James            Lee             24           Math
    3          Ricky         Martin             47          Music

:select * from students where age > 20
    #          first           last            age          major
-----------------------------------------------------------------
    1            Joe           Blow             23             CS
    4          James            Lee             24           Math
    3          Ricky         Martin             47          Music

:select * from students where age < 20
    #          first           last            age          major
-----------------------------------------------------------------
    0          Lydia          Smith             19        Physics

:select * from students where age <= 20
    #          first           last            age          major
-----------------------------------------------------------------
    0          Lydia          Smith             19        Physics
    2        Richard           Wang             20             CS

:select * from students where age >= 21
    #          first           last            age          major
-----------------------------------------------------------------
    1            Joe           Blow             23             CS
    4          James            Lee             24           Math
    3          Ricky         Martin             47          Music

:select * from students where age > 21
    #          first           last            age          major
-----------------------------------------------------------------
    1            Joe           Blow             23             CS
    4          James            Lee             24           Math
    3          Ricky         Martin             47          Music

:select * from students where age < 21
    #          first           last            age          major
-----------------------------------------------------------------
    0          Lydia          Smith             19        Physics
    2        Richard           Wang             20             CS

:select * from students where age <= 21
    #          first           last            age          major
-----------------------------------------------------------------
    0          Lydia          Smith             19        Physics
    2        Richard           Wang             20             CS

:select * from students where age >= 20 and last > Blow
    #          first           last            age          major
-----------------------------------------------------------------
    2        Richard           Wang             20             CS
    3          Ricky         Martin             47          Music
    4          James            Lee             24           Math

:select * from students where age >= 21 and major = CS
    #          first           last            age          major
-----------------------------------------------------------------
    1            Joe           Blow             23             CS

:select * from students where age = 20 or major = Music
    #          first           last            age          major
-----------------------------------------------------------------
    2        Richard           Wang             20             CS
    3          Ricky         Martin             47          Music

:select first, major from students where age = 26
    #          first          major
-----------------------------------

:select first, major from students where major = CS
    #          first          major
-----------------------------------
    1            Joe             CS
    2        Richard             CS

:
Process finished with exit code 0
```

### Songs list

```
"C:\Users\Evelyn\Google Drive (ylu40@go.pasadena.edu)\CS 8\Projects\0520_Final\0520_Final\cmake-build-debug\0520_Final.exe"

:
:create table songs fields title, artist, album
SQL: [songs] is created.

:insert into songs values "Eletric Feel", "MGMT", "Oracular Spectacular"
SQL: inserted record into [songs]

:insert into songs values "Kids", "MGMT", "Oracular Spectacular"
SQL: inserted record into [songs]

:insert into songs values "Los Angeles", "St. Vincent", "Masseducation"
SQL: inserted record into [songs]

:insert into songs values "Prince Johnny", "St. Vincent", "St. Vincent (Deluxe)"
SQL: inserted record into [songs]

:insert into songs values "L$D", "A$AP Rocky", "AT.LONG.LAST.A$AP"
SQL: inserted record into [songs]

:insert into songs values Sunflower, "Post Malone", "Spider-Man"
SQL: inserted record into [songs]

:select * from songs
    #                    title                   artist                    album
--------------------------------------------------------------------------------
    0             Eletric Feel                     MGMT     Oracular Spectacular
    1                     Kids                     MGMT     Oracular Spectacular
    2              Los Angeles              St. Vincent            Masseducation
    3            Prince Johnny              St. Vincent     St. Vincent (Deluxe)
    4                      L$D               A$AP Rocky        AT.LONG.LAST.A$AP
    5                Sunflower              Post Malone               Spider-Man

:select * from songs where artist = "MGMT"
    #                    title                   artist                    album
--------------------------------------------------------------------------------
    0             Eletric Feel                     MGMT     Oracular Spectacular
    1                     Kids                     MGMT     Oracular Spectacular

:select * from songs where album = "Oracular Spectacular"
    #                    title                   artist                    album
--------------------------------------------------------------------------------
    0             Eletric Feel                     MGMT     Oracular Spectacular
    1                     Kids                     MGMT     Oracular Spectacular

:select * from songs where artist = 'St. Vincent'
    #                    title                   artist                    album
--------------------------------------------------------------------------------
    2              Los Angeles              St. Vincent            Masseducation
    3            Prince Johnny              St. Vincent     St. Vincent (Deluxe)

:select artist from songs where title = Sunflower
    #                   artist
------------------------------
    5              Post Malone

:select song, album from songs where artist = "ASAP Rocky"
    #                    title                    album
-------------------------------------------------------

:select song, album from songs where artist = "A$AP Rocky"
    #                    title                    album
-------------------------------------------------------
    4                      L$D        AT.LONG.LAST.A$AP

:
Process finished with exit code 0
```