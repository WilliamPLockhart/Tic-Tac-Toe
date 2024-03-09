#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include "hash.hpp"

class tiles
{
public:
    tiles();
    ~tiles();
    std::string getTiles(int needed);

private:
    std::string available_letters = "AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ";
    Hash t;
};