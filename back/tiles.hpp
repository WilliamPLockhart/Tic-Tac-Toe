#pragma once
#include <iostream>
#include <fstream>
#include <random>

class tiles
{
public:
    tiles();
    std::string getTiles(int needed, bool infinite = false);

private:
    // add blank tiles
    std::string available_letters = "AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ..";
};