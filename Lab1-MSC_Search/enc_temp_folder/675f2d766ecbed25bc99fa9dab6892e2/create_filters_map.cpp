#include "create_filters_map.hpp"

using namespace std;
using namespace Config;
using namespace Utils;

namespace FiltersMap {
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

        vector<string> filters = read_lines_from_file(MSC_OUTPUT_FILENAME);
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
                string filtered_word;
                int ones_count = 0;

                for (size_t j = 0; j < SEARCH_WORD_SIZE; ++j) {
                    if (j < filter.size() && filter[j] == '1') {
                        filtered_word += window[j];
                        ones_count++;

                        // Stop if we reached the required amount of 1s, and fill with underscores
                        if (ones_count == FILTER_AMOUNT_OF_MATCHES) {
                            filtered_word.append(SEARCH_WORD_SIZE - filtered_word.length(), '_');
                            break;
                        }
                    }
                    else {
                        filtered_word += '_';
                    }
                }

                // === Phase 4: Update the map ===
                size_t position = i + 1;

                if (filters_map.find(filtered_word) == filters_map.end()) {
                    filters_map[filtered_word] = WordMatch{ filtered_word, {position} };
                }
                else {
                    filters_map[filtered_word].positions.push_back(position);
                }
            }
            print_progress(static_cast<int>(i + 1), static_cast<int>(total_iterations));
        }

        // === Phase 5: Convert to output lines and save to output file ===
        vector<WordMatch> results;
        for (const auto& pair : filters_map) {
            results.push_back(pair.second);
        }

        vector<string> lines = convert_matches_to_lines(results);
        int status = save_to_file(lines, FILTERS_MAP);

        if (status == 0) cout << "[FiltersMapBuilder] Filters map creation complete.\n";
        return status;
    }

}
