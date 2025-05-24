#include "create_filters_map.hpp"

using namespace std;
using namespace Config;
using namespace Utils;

namespace FiltersMap {

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

        string filtered_word(word.size(), '_');

        // --- Apply filter to substring of word ---
        for (size_t i = 0; i < filter_length; ++i)
            if (filter[i] == '1')
                filtered_word[i] = word[sliding_window_index + i];

        return filtered_word;
    }

    unordered_map<string, WordMatch> filters_map;

    /**
     * @brief Creates a map of filtered words and their positions in the input text.
     *        The process involves reading text and filters, applying the filters over
     *        sliding windows in the text, building a map of filtered words to positions,
     *        and saving the result to a file.
     *
     * @return unordered_map<string, WordMatch> The constructed filters map.
     *         Returns an empty map on failure.
     */
    int create_filters_map() {
        cout << "[FiltersMapBuilder] Starting filters map creation...\n";

        // === Phase 1: Read files ===
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[FiltersMapBuilder] Failed to load text.\n";
            return -1;
        }

        vector<string> filters = read_lines_from_file(STANDARD_MCS_OUTPUT_FILENAME);
        if (filters.empty()) {
            cerr << "[FiltersMapBuilder] Failed to load MCS filters.\n";
            return -1;
        }

        // === Phase 2: Iterate through text ===
        filters_map.clear();
        size_t text_len = text.length();
        size_t total_iterations = text_len - SEARCH_WORD_SIZE + 1;

        for (size_t i = 0; i < total_iterations; ++i) {
            string window = text.substr(i, SEARCH_WORD_SIZE);

            // === Phase 3: Create words from the window using each filter ===
            for (const string& filter : filters) {
                string filtered_word = applyFilterToWord(window, filter, 0);

                // === Phase 4: Update the map ===
                size_t position = i + 1;

                if (filters_map.find(filtered_word) == filters_map.end())
                    filters_map[filtered_word] = WordMatch{ filtered_word, {position} };
                else 
                    filters_map[filtered_word].positions.insert(position);
            }
            print_progress(static_cast<int>(i + 1), static_cast<int>(total_iterations));
        }

        // === Phase 5: Convert to output lines and save to output file ===
        cout << "Should also save to file: " << FILTERS_MAP << "? (y/n)\n";
        int status = 0;
        char choice;
        cin >> choice;
        if (choice == 'y') {
            vector<WordMatch> results;
            size_t total = filters_map.size();
            size_t i = 0;
            cout << "[FiltersMapBuilder] Organising Data..\n";
            for (const auto& pair : filters_map) {
                results.push_back(pair.second);
                print_progress(static_cast<int>(++i), static_cast<int>(total));
            }

            vector<string> lines = convert_matches_to_lines(results);
            status = save_to_file(lines, FILTERS_MAP);
        }

        if (status == 0) cout << "[FiltersMapBuilder] Filters map creation complete.\n";
        return status;
    }

}
