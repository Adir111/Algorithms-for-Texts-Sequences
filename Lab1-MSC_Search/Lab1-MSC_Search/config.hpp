#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <vector>

namespace Config {

    const int TEXT_SIZE = 1000000; // 1 million characters
    const int NUMBER_OF_SEARCH_WORDS = 10000; // Number of search words (taken from the start of the text)
    const int SEARCH_WORD_SIZE = 21; // size of word for search
    const int MINIMAL_MATCHES = 15; // minimal amount of matches
    const int FILTER_AMOUNT_OF_MATCHES = 4; // amount of matches in the filter
    const char Y_LETTER = 't'; // the start char the text is being generated from (up to z)
    const std::string STANDARD_MCS_OUTPUT_FILENAME = "standard_mcs_output"; // Output file for mcs
    const std::string POSITIONAL_MCS_OUTPUT_FILENAME = "positional_mcs_output"; // Output file for positional mcs
    const std::string RANDOM_GENERATED_TEXT_FILENAME = "generated_text"; // Output file for random generated text
    const std::string SEARCH_WORDS_FILENAME = "search_words"; // Output file for search words
    const std::string FILTERS_MAP = "filters_map"; // Output file for filters map
    const std::string NAIVE_SEARCH_OUTPUT_FILENAME = "naive_output"; // Output file for naive search algorithm
    const std::string STANDARD_MCS_SEARCH_OUTPUT_FILENAME = "standard_mcs_search_output"; // Output file for standard MCS search algorithm
    const std::string POSITIONAL_MCS_SEARCH_OUTPUT_FILENAME = "positional_mcs_search_output"; // Output file for positional MCS search algorithm
    const bool VALIDATE_SELECTIONS = false; // Flag indicating if should validate user selections

    const int PROGRESS_UPDATES_COUNT = 10000; // Used to control how often progress is updated during a loop
    const std::vector<int> TIMER_CHOICES = { 6, 7, 8 }; // Used to indicate which choices should have timer
    const std::vector<std::string> OPTIONS = {
        "MCS Creation",
        "Positional MCS Creation",
        "Random Text Creation",
        "Search Words Creation",
        "Filters Map Creation",
        "Naive Search",
        "Standard MCS Search",
        "Positional MCS Search"
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