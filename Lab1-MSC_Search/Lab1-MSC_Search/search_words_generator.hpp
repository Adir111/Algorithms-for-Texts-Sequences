#ifndef SEARCH_WORDS_GENERATOR_HPP
#define SEARCH_WORDS_GENERATOR_HPP

#include <string>
#include <vector>
#include "config.hpp"
#include "utils.hpp"

namespace SearchWordsGenerator {

    /**
     * @brief Generates a list of search words from the random text file
     *        and saves them into a target file.
     *
     * This function:
     * - Loads the random text from the file defined in config.
     * - Extracts non-overlapping search words of length SEARCH_WORD_SIZE.
     * - Extracts NUMBER_OF_SEARCH_WORDS such words.
     * - Saves them to the file defined by SEARCH_WORDS_FILENAME.
     *
     * @return int 0 on success, -1 on failure
     */
    int generate_search_words();
}

#endif // SEARCH_WORDS_GENERATOR_HPP
