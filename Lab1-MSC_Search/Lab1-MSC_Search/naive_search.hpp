#ifndef NAIVE_SEARCH_HPP
#define NAIVE_SEARCH_HPP

#include <string>
#include <vector>
#include <set>
#include <chrono>

#include "config.hpp"
#include "utils.hpp"
#include "WordMatch.hpp"
#include "Summary.hpp"

namespace NaiveSearch {

    /**
     * @brief Checks if a substring of the given text matches the target word
     *        with at least a minimum number of character matches.
     *
     * This function compares the substring of `text` starting at `text_index`
     * with the `word`, character by character. It counts how many characters
     * match at the corresponding positions. If the number of matching characters
     * is greater than or equal to `MINIMAL_MATCHES`, the function returns true.
     *
     * @param text The full text in which to search for a partial match.
     * @param text_index The starting index in the text where the comparison begins.
     * @param word The word to compare against the substring of the text.
     * @return true if the number of matching characters is >= MINIMAL_MATCHES;
     *         false otherwise.
     *
     * @note Assumes that `text_index + word.length()` does not exceed `text.size()`.
     */
    bool check_matches(const string& text, int text_index, const string& word);

    /**
     * @brief Performs naive search for all given words and writes results to file.
     *
     * @return int 0 on success, -1 on failure
     */
    int perform_naive_search();
}

#endif