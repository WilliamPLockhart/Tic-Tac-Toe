#include "tiles.hpp"

tiles::tiles()
{
}

std::string tiles::getTiles(int needed, bool infinite)
{
    std::string value;
    // makes sure that needed is not greater than the available letters
    if (needed > available_letters.size() && infinite == false)
    {
        needed = available_letters.size();
    }
    for (int i = 0; i < needed; i++)
    {
        // generates a random int from 0 to the string size-1
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, available_letters.size() - 1);
        // stores random vale
        int randomInt = distribution(gen);
        // adds the letter to the string and removes from list
        value += available_letters.at(randomInt);
        if (infinite == false)
        {
            available_letters.erase(available_letters.begin() + randomInt);
        }
    }
    return value;
}