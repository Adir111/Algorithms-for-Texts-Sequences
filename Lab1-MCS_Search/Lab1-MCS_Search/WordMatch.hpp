#ifndef WORD_MATCH_HPP
#define WORD_MATCH_HPP

#include <string>
#include <vector>
#include <set>

using std::string;
using std::vector;
using std::set;

/**
 * @brief Represents a word and its positions in text.
 */
struct WordMatch {
    string word;
    set<size_t> positions;

    bool operator<(const WordMatch& other) const {
        return word < other.word;
    }

    /**
     * @brief Returns a string representation of the WordMatch object.
     *
     * @return A string that represents the WordMatch.
     */
    string to_string() const {
        string line = "Word: " + word + "\nPositions: ";

        // Iterate over the set of positions and concatenate the string
        size_t count = 0;
        for (auto pos : positions) {
            line += std::to_string(pos);
            if (++count < positions.size()) {
                line += ", ";
            }
        }

        return line;
    }
};

#endif
