#ifndef WORD_MATCH_HPP
#define WORD_MATCH_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * @brief Represents a word and its positions in text.
 */
struct WordMatch {
    string word;
    vector<int> positions; // index of positions start from 1 (not 0)
};

#endif