#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <vector>

namespace Config {

    const int TEXT_SIZE = 1000000; // 1 million characters
    const int NUMBER_OF_SEARCH_WORDS = 1000; // Number of search words (taken from the start of the text)
    const int NUMBER_OF_VARIATIONS = 10; // Number of variations of search word inserted into the text
    const int SEARCH_WORD_SIZE = 21; // size of word for search
    const int MINIMAL_MATCHES = 15; // minimal number of matches
    const int FILTER_NUMBER_OF_MATCHES = 6; // number of matches in the filter
    const char Y_LETTER = 'a'; // the start char the text is being generated from (up to z)
    const size_t SIZE = 'z' - Y_LETTER + 2; // Size of positions array, an extra position for $

    const std::string MCS_OUTPUT_FILENAME = "mcs_output"; // Output file for mcs
    const std::string RANDOM_GENERATED_TEXT_FILENAME = "generated_text"; // Output file for random generated text
    const std::string SEARCH_WORDS_FILENAME = "search_words"; // Output file for search words
    const std::string MCS_TREE_SEARCH_OUTPUT_FILENAME = "tree_mcs_search_output"; // Output file for tree MCS search algorithm
    const std::string MCS_TREE_SEARCH_SUMMARY_FILENAME = "summary_tree_mcs"; // Output file for summary of Tree MCS Search
    const std::string COMBINED_SUMMARY_FILENAME = "summary_all"; // Output file for combined summary of all searches

    const int PROGRESS_UPDATES_COUNT = 10000; // Used to control how often progress is updated during a loop
    const std::vector<std::string> OPTIONS = {
        "MCS Creation",
        "Random Text and Search Words Creation",
        "MCS Tree Creation",
        "MCS Tree Search",
        "Create Final Summary"
    }; // List of choices for user

    /**
     * @brief Prints the program configuration
     */
    void print_config();

    /**
     * @brief Validates configuration constants. Collects all config issues and exits if any found.
     */
    void validate_config();
}

#endif