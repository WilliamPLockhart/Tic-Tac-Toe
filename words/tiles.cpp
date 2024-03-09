#include "tiles.hpp"

tiles::tiles()
{
}

std::string tiles::getTiles(int needed)
{
    std::string value;
    if (needed > available_letters.size())
    {
        needed = available_letters.size();
    }
    for (int i = 0; i < needed; i++)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, available_letters.size() - 1);
        int randomInt = distribution(gen);
        value += available_letters.at(randomInt);
        available_letters.erase(available_letters.begin() + randomInt);
    }
    return value;
}