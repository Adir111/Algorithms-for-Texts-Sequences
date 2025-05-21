#include "standard_mcs_search.hpp"

using namespace std;
using namespace Utils;
using namespace Config;
using namespace FiltersMap;

namespace StandardMCSSearch {
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

        // Load filters map
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
                    string filtered_word(search_word_length, '_');  // fill with '_'

                    // --- Apply filter to substring of word ---
                    for (size_t i = 0; i < filter_length; ++i) {
                        if (filter[i] == '1') {
                            filtered_word[sliding_window_index + i] = word[sliding_window_index + i];
                        }
                    }

                    // --- Check if this masked word appears in the filters map ---
                    auto it = filters_map.find(filtered_word);
                    if (it != filters_map.end()) {
                        for (size_t pos : it->second.positions) {
                            size_t position_index = pos - 1;
                            int matches = 0;
                            if (position_index + search_word_length >= TEXT_SIZE) // to make sure not overflowing the text last pointer
                                continue; // skip position
                            
                            for (int check_index = 0; check_index < search_word_length; check_index++) // Check each chars surrounding the index we got in the text.
                                if (word[check_index] == text[position_index + check_index - sliding_window_index])
                                    matches++;

                            if (matches >= MINIMAL_MATCHES)
                                insert_or_update_match(results, word, pos);
                        }
                    }
                }
            }
            print_progress(static_cast<int>(word_index), static_cast<int>(total_words));
        }

        vector<WordMatch> results_vector(results.begin(), results.end());
        vector<string> output_lines = convert_matches_to_lines(results_vector);

        // Save results to file
        int status = save_to_file(output_lines, STANDARD_MCS_OUTPUT_FILENAME);
        if (status == 0) cout << "[StandardMCSSearch] MCS search complete. Results saved to " << STANDARD_MCS_OUTPUT_FILENAME << '\n';
        return status;
    }
}