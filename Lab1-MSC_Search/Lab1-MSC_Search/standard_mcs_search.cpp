#include "standard_mcs_search.hpp"

using namespace std;
using namespace Utils;
using namespace Config;
using namespace FiltersMap;

namespace StandardMCSSearch {

    /**
     * @brief Applies a binary filter to a given word and returns a filtered version of the word.
     *
     * The method creates a filtered version of the input `word`, where characters corresponding
     * to '1' positions in the filter are retained, and all others are replaced with underscores ('_').
     *
     * @param word The word to be filtered.
     * @param filter A string of '0's and '1's representing the filter. '1' means the character at that position
     *               will be kept from the original word, and '0' means it will be replaced with '_'.
     * @param sliding_window_index The starting index within the word to begin applying the filter.
     *
     * @return A filtered string where the characters are either from the `word` or '_' depending on the filter.
     */
    string applyFilterToWord(const string& word, const string& filter, size_t sliding_window_index) {
        size_t search_word_length = word.size();
        size_t filter_length = filter.size();

        string filtered_word(search_word_length, '_');  // Fill with '_'

        // --- Apply filter to substring of word ---
        for (size_t i = 0; i < filter_length; ++i) {
            if (filter[i] == '1') {
                filtered_word[sliding_window_index + i] = word[sliding_window_index + i];
            }
        }

        return filtered_word;
    }

    /**
     * @brief Searches for matches of a filtered word in a given text and returns the total count of matches found.
     *
     * This method searches for occurrences of `filtered_word` in the global `filters_map` and compares the characters
     * in surrounding positions of the `text` to count the matches. If the number of matches exceeds the defined threshold,
     * it updates the `results` vector with the matching positions and increments the total count of finds.
     *
     * @param filtered_word The word after applying the filter, which is to be searched.
     * @param word The original word being checked for matches.
     * @param text The text to search for matches in.
     * @param sliding_window_index The index used for adjusting word positions in the text.
     * @param results A set to store the matches.
     * @return The total count of finds (matches found).
     */
    int searchAndInsertMatches(const string& filtered_word, const string& word, const string& text,
        size_t sliding_window_index, set<WordMatch>& results) {
        int count_total_finds = 0;

        // Search the filtered_word in the filters_map
        auto it = filters_map.find(filtered_word);
        if (it != filters_map.end()) {
            // Iterate over the positions where the filtered word was found
            for (size_t pos : it->second.positions) {
                size_t position_index = pos - 1;
                int matches = 0;

                // Ensure we're not overflowing the text by checking boundaries, skip position if does
                if (position_index + word.size() >= TEXT_SIZE)
                    continue;

                // Check each character surrounding the position in the text
                for (int check_index = 0; check_index < word.size(); ++check_index) {
                    if (word[check_index] == text[position_index + check_index - sliding_window_index])
                        matches++;
                }

                // If matches exceed the threshold, insert or update the match
                if (matches >= MINIMAL_MATCHES) {
                    insert_or_update_match(results, word, pos);
                    count_total_finds++;
                }
            }
        }
        return count_total_finds;
    }


    /**
     * @brief Executes a standard MCS search using filters and a filters map.
     *
     * The method processes each search word and applies all MCS filters by sliding them across the word.
     * For every valid application of a filter, it generates a masked word (with '_' in unmatched positions),
     * looks it up in the filters map, and collects matching positions.
     * Finally, the results are saved to the file defined in the config under STANDARD_MCS_OUTPUT_FILENAME.
     *
     * @return 0 on success, -1 on failure (e.g., failed to load files or save output)
     */
    int run_standard_mcs_search() {
        cout << "[StandardMCSSearch] Starting usual MCS search...\n";
        int count_total_finds = 0;

        // Load Text
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[StandardMCSSearch] Failed to load text - its empty or doesn`t exist.\n";
            return -1;
        }

        // Load MCS filters
        vector<string> mcs_filters = read_lines_from_file(STANDARD_MCS_OUTPUT_FILENAME);
        if (mcs_filters.empty()) {
            cerr << "[StandardMCSSearch] Failed to load MCS filters - its empty or doens`t exist.\n";
            return -1;
        }

        // Load search words
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[StandardMCSSearch] Failed to load search words.\n";
            return -1;
        }

        // Verify filters map exist
        if (filters_map.empty()) {
            cerr << "[StandardMCSSearch] Failed to load filters map - its empty.\n";
            return -1;
        }

        // --- Prepare container to collect results ---
        set<WordMatch> results;
        size_t total_words = search_words.size();

        // --- Iterate over each search word ---
        for (size_t word_index = 0; word_index < total_words; ++word_index) {
            const string& word = search_words[word_index];
            const size_t search_word_length = word.length();

            // --- Try all filters on this word ---
            for (const string& filter : mcs_filters) {
                size_t filter_length = filter.length();

                // --- Slide the filter over the word ---
                for (size_t sliding_window_index = 0; sliding_window_index + filter_length <= search_word_length; ++sliding_window_index) {
                    string filtered_word = applyFilterToWord(word, filter, sliding_window_index);

                    // --- Check if this masked word appears in the filters map ---
                    count_total_finds += searchAndInsertMatches(filtered_word, word, text, sliding_window_index, results);
                }
            }
            print_progress(static_cast<int>(word_index), static_cast<int>(total_words));
        }

        vector<WordMatch> results_vector(results.begin(), results.end());
        vector<string> output_lines = convert_matches_to_lines(results_vector);

        // Save results to file
        int status = save_to_file(output_lines, STANDARD_MCS_OUTPUT_FILENAME);
        if (status == 0) cout << "[StandardMCSSearch] MCS search complete with total finds " << count_total_finds << ".Results saved to " << STANDARD_MCS_OUTPUT_FILENAME << '\n';
        return status;
    }
}