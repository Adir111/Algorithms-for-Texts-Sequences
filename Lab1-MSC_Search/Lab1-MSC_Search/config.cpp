#include "config.hpp"

using namespace std;

namespace Config {
    /**
     * @brief Prints the program configuration
     */
    void print_config() {
        cout << "================ Program Configuration ================\n";
        cout << "Text size                   : " << TEXT_SIZE << "\n";
        cout << "Number of Search Words      : " << NUMBER_OF_SEARCH_WORDS << "\n";
        cout << "Y-Letter                    : " << Y_LETTER << "\n";
        cout << "Search_Word_Size (pattern)  : " << SEARCH_WORD_SIZE << "\n";
        cout << "Minimal_Matches (min hits)  : " << MINIMAL_MATCHES << "\n";
        cout << "Filter_Amount_Of_Matches    : " << FILTER_AMOUNT_OF_MATCHES << "\n";
        cout << "MSC_Output_FileName         : " << MSC_OUTPUT_FILENAME << "\n";
        cout << "Text_Output_Filename        : " << RANDOM_GENERATED_TEXT_FILENAME << "\n";
        cout << "Search_Words_Filename       : " << SEARCH_WORDS_FILENAME << "\n";
        cout << "Naive_Output_Filename       : " << NAIVE_SEARCH_OUTPUT_FILENAME << "\n";
        cout << "=======================================================\n\n";
    }

    /**
     * @brief Validates configuration constants. Collects all config issues and exits if any found.
     */
    void validate_config() {
        vector<string> errors;

        const int minimumTextSize = 100000;
        const int maxSearchWordSizeFactor = 6;

        if (SEARCH_WORD_SIZE < MINIMAL_MATCHES) {
            errors.emplace_back("MINIMAL_MATCHES should be lower than SEARCH_WORD_SIZE");
        }

        if (MINIMAL_MATCHES < FILTER_AMOUNT_OF_MATCHES) {
            errors.emplace_back("FILTER_AMOUNT_OF_MATCHES should be lower than MINIMAL_MATCHES");
        }

        if (TEXT_SIZE < minimumTextSize) {
            errors.emplace_back("TEXT_SIZE is too small, should be higher than " + to_string(minimumTextSize));
        }

        if (!(Y_LETTER >= 'a' && Y_LETTER <= 'z')) {
            errors.emplace_back("Y_LETTER should be a lowercase English letter");
        }

        if ((SEARCH_WORD_SIZE * NUMBER_OF_SEARCH_WORDS) > (TEXT_SIZE * maxSearchWordSizeFactor)) {
            errors.emplace_back("SEARCH_WORD_SIZE * NUMBER_OF_SEARCH_WORDS should not exceed TEXT_SIZE * " + to_string(maxSearchWordSizeFactor));
        }

        if (!errors.empty()) {
            cerr << "Configuration errors:\n";
            for (const auto& err : errors) {
                cerr << "- " << err << "\n";
            }
            exit(EXIT_FAILURE);
        }
    }
}