#pragma once
#include <iostream>
#include <fstream>
#include <time.h>
#define table_size 214433
class Hash
{
private:
    std::string maxWord;
    std::string *hash_table;
    int max_searches = -1;
    double total_col = 0;

public:
    Hash();
    ~Hash();
    int HashFunc1(std::string word);
    int HashFunc2(std::string word);
    void insert(std::string word);
    bool search(std::string word);
    void printMax();
    void printMaxWord();
};