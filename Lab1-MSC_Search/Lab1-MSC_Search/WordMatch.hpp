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
    vector<size_t> positions;

    bool operator<(const WordMatch& other) const {
        return word < other.word;
    }
};

#endif
