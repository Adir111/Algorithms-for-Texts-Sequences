#include "search_words_generator.hpp"

using namespace std;
using namespace Config;
using namespace Utils;

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
    int generate_search_words() {
        cout << "[SearchWords] Generating " << NUMBER_OF_SEARCH_WORDS << " Search Words...\n";
        // Step 1: Load full text content
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[SearchWords] Failed to load text content.\n";
            return -1;
        }

        // Step 2: Validate text is long enough
        size_t requiredLength = SEARCH_WORD_SIZE * NUMBER_OF_SEARCH_WORDS;
        if (text.length() < requiredLength) {
            cerr << "[SearchWords] Text is too short for generating required search words.\n";
            return -1;
        }

        // Step 3: Generate vector of search words
        vector<string> searchWords;
        size_t i = 0;

        while (searchWords.size() < NUMBER_OF_SEARCH_WORDS && i + SEARCH_WORD_SIZE <= text.length()) {
            string word = text.substr(i, SEARCH_WORD_SIZE);
            searchWords.push_back(word);
            i += SEARCH_WORD_SIZE;  // Move to next non-overlapping segment

            print_progress(static_cast<int>(searchWords.size()), static_cast<int>(NUMBER_OF_SEARCH_WORDS));
        }
        print_progress(static_cast<int>(NUMBER_OF_SEARCH_WORDS), static_cast<int>(NUMBER_OF_SEARCH_WORDS));
        cout << '\n';

        // Step 4: Save the vector to a file
        int status = save_to_file(searchWords, SEARCH_WORDS_FILENAME);
        if (status == 0) cout << "[SearchWords] Successfully generated and saved " << searchWords.size() << " search words.\n";
        return status;
    }

}
