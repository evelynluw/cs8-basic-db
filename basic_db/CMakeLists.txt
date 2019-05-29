cmake_minimum_required(VERSION 3.14)
project(0520_Final)

set(CMAKE_CXX_STANDARD 14)

add_executable(0520_Final
        main.cpp
#        libs/array_utilities.h
#        libs/BPTree.h
#        libs/const.h
#        libs/Map.h
#        libs/MMap.h
        libs/stokenizer.h
        libs/stokenizer.cpp
        Parser.h
        SQL.h
        Table.h SQL.cpp Table.cpp Parser.cpp Record.h Record.cpp)