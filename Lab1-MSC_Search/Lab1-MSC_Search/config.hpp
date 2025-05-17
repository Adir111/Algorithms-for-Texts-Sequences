#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>
#include <vector>

namespace Config {

    const int TEXT_SIZE = 10000000; // 10 million characters
    const int NUMBER_OF_SEARCH_WORDS = 10000; // Number of search words (taken from the start of the text)
    const int SEARCH_WORD_SIZE = 21; // size of word for search
    const int MINIMAL_MATCHES = 15; // minimal amount of matches
    const int FILTER_AMOUNT_OF_MATCHES = 4; // amount of matches in the filter
    const char Y_LETTER = 'k'; // the start char the text is being generated from (up to z)
    const std::string MSC_OUTPUT_FILENAME = "msc_output"; // Output file for msc
    const std::string RANDOM_GENERATED_TEXT_FILENAME = "generated_text"; // Output file for random generated text
    const std::string SEARCH_WORDS_FILENAME = "search_words"; // Output file for search words
    const std::string NAIVE_SEARCH_OUTPUT_FILENAME = "naive_output"; // Output file for naive search algorithm
    const std::string FILTERS_MAP = "filters_map"; // Output file for filters map
    const bool VALIDATE_SELECTIONS = true; // Flag indicating if should validate user selections

    const int PROGRESS_UPDATES_COUNT = 10000; // Used to control how often progress is updated during a loop
    const vector<std::string> choices = {
        "MSC Creation",
        "Random Text Generation",
        "Search Words Generation",
        "Naive Search",
        "Filters Map"
    };

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