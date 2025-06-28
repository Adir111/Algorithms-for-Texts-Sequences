#ifndef SEARCH_WORDS_AND_TEXT_GENERATOR_HPP
#define SEARCH_WORDS_AND_TEXT_GENERATOR_HPP

#include <string>
#include <vector>
#include "config.hpp"
#include "utils.hpp"
#include <random>
#include <unordered_set>

namespace SearchWordsAndTextGenerator {

    /**
     * @brief Generates search words and random text, then saves them to files.
     *
     * This function performs two tasks:
     * 1. It generates a list of random search words and saves them to a file.
     * 2. It generates random text of size `TEXT_SIZE` and inserts the search words and their variants
     *    at random positions in the text. After inserting all words and variants, the remaining spaces
     *    are filled with random characters. Finally, the text is saved to a file.
     *
     * This function ensures that the total size of the search words (including variants) does not exceed
     * the size of the text. If the size condition is violated, it prints an error and terminates the program.
     *
     * @return int 0 on success, -1 on failure.
     */
    int generate_text_and_search_words();
}

#endif
