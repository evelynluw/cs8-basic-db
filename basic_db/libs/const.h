/*
 * Author: Evelyn Lu
 * Project: File Tokenizer
 * Purpose: Tokenize a text file
 * File: const.h declare consts
*/
#ifndef INC_0304_STRING_TOKENIZER_CONST_H
#define INC_0304_STRING_TOKENIZER_CONST_H

#include <map>

using namespace std;

const map<int, string> types_map = //machine starting state and type_name string
        //can use enum too, with successor function?
        {
            {-1, "BAD CHAR"},
            {0, "NUMBER"},
            {4, "ALPHA"},
            {6, "SPACE"},
            {8, "PUNC"},
            {28, "UNKNOWN"}
        };

const int DEBUG = false;

const int BLOCK_SIZE = 500;

#endif //INC_0304_STRING_TOKENIZER_CONST_H
