#include "naive_search.hpp"

using namespace std;
using namespace Config;
using namespace Utils;

namespace NaiveSearch {
    /**
     * @brief Finds all starting positions of a word in the given text using naive search.
     *
     * @param text The full text to search in.
     * @param word The word to search for.
     * @return vector<int> List of 1-based starting positions.
     */
    static vector<int> find_word_positions(const string& text, const string& word) {
        vector<int> positions;
        size_t text_len = text.length();
        size_t word_len = word.length();

        // Loop through the text to find matches
        for (size_t i = 0; i <= text_len - word_len; ++i) {
            bool match = true;

            // Check each character of the word
            for (size_t j = 0; j < word_len; ++j) {
                if (text[i + j] != word[j]) {
                    match = false;
                    break;
                }
            }

            // If match is found, store the position (1-based index)
            if (match) {
                positions.push_back(static_cast<int>(i + 1));
            }
        }

        return positions;
    }

    /**
     * @brief Performs naive search for all given words and writes results to file.
     *
     * @param search_words Vector of words to search
     * @param output_filename Output file name
     * @return int 0 on success, -1 on failure
     */
    int perform_naive_search() {
        cout << "[NaiveSearch] Starting naive search...\n";

        // Load the text to search from file
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[NaiveSearch] Failed to load input text.\n";
            return -1;
        }

        // Load search words from file
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[NaiveSearch] Failed to load search words.\n";
            return -1;
        }

        unordered_map<string, WordMatch> unique_matches;

        // Search each unique word only once
        for (const auto& word : search_words) {
            if (unique_matches.find(word) == unique_matches.end()) {
                vector<int> positions = find_word_positions(text, word);
                unique_matches[word] = WordMatch{ word, positions };
            }
        }

        // Prepare output lines
        vector<WordMatch> result;
        for (const auto& pair : unique_matches) {
            result.push_back(pair.second);
        }

        vector<string> lines = convert_matches_to_lines(result);
        int status = save_to_file(lines, NAIVE_SEARCH_OUTPUT_FILENAME);

        cout << "[NaiveSearch] Search complete.\n";
        return status;
    }
}
