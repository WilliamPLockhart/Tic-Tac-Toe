#include "hash.hpp"
Hash::Hash()
{
    // allocates memory to hash_table, opens dictionary.txt and parses the words into the table
    hash_table = new std::string[table_size];
    std::ifstream inputfile;
    inputfile.open("dictionary.txt");
    if (inputfile.fail())
    {
        std::cout << "error opening file\n";
    }
    std::string word;
    while (std::getline(inputfile, word))
    {
        insert(word);
    }
    std::cout << "avg collisions per word: " << total_col / 178692 << std::endl;
}

Hash::~Hash()
{
    delete[] hash_table;
}

int Hash::HashFunc1(std::string word)
{
    int hashValue = 0;
    for (char ch : word)
    {
        // 1303 426 3.46685
        hashValue = (hashValue * 1303 + ch) % table_size;
    }
    return hashValue;
}

int Hash::HashFunc2(std::string word)
{
    int hashValue = 0;
    for (char ch : word)
    {
        // 1307
        hashValue = (hashValue * 1307 + ch) % table_size;
    }
    return (hashValue == 0) ? 1 : hashValue;
}

void Hash::insert(std::string word)
{
    int index = HashFunc1(word);
    int stepSize = HashFunc2(word);
    int attempt = 1;
    while (!hash_table[index].empty())
    {
        // Collision occurred, move to the next position using double hashing
        index = (index + stepSize * attempt) % table_size;
        attempt += 1;
        total_col += 1;
    }
    // finds the max number of searches so that later on it is faster to determine if a word is in the table
    if (attempt > max_searches)
    {
        max_searches = attempt;
        maxWord = word;
    }
    // places word in hash
    hash_table[index] = word;
    // std::cout << "inserting " << word << " at index: " << index << std::endl;
}

bool Hash::search(std::string word)
{
    int index = HashFunc1(word);
    int stepSize = HashFunc2(word);
    int attempt = 1;
    int count = 1;

    // once count is above max_searches, it is known the word is not in the table
    while (attempt <= max_searches)
    {
        if (!hash_table[index].empty())
        {
            // redundency
            if (index < 0 || index >= table_size)
            {
                std::cerr << "Error: Invalid index calculated. Aborting search." << std::endl;
                return false;
            }

            // Check if the current position contains the target word
            if (hash_table[index] == word)
            {
                return true; // Word found
            }

            // Move to the next position using double hashing
            index = (index + stepSize * attempt) % table_size;
            attempt += 1;
        }
        else
        {
            break;
        }
    }
    return false; // Word not found
}

void Hash::printMax()
{
    std::cout << "max conflicts: " << max_searches << std::endl;
}

void Hash::printMaxWord()
{
    std::cout << "max word: " << maxWord << std::endl;
}