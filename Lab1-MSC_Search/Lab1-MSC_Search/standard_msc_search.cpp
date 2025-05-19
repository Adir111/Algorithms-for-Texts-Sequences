#include "standard_msc_search.hpp"

using namespace std;
using namespace Utils;
using namespace Config;
using namespace FiltersMap;

namespace StandardMSCSearch {
    /**
     * @brief Executes a standard MSC search using filters and a filters map.
     *
     * The method processes each search word and applies all MSC filters by sliding them across the word.
     * For every valid application of a filter, it generates a masked word (with '_' in unmatched positions),
     * looks it up in the filters map, and collects matching positions.
     * Finally, the results are saved to the file defined in the config under STANDARD_MCS_OUTPUT_FILENAME.
     *
     * @return 0 on success, -1 on failure (e.g., failed to load files or save output)
     */
    int run_standard_msc_search() {
        cout << "[StandardMSCSearch] Starting usual MSC search...\n";

        // Load MCS filters
        vector<string> filters = read_lines_from_file(MSC_OUTPUT_FILENAME);
        if (filters.empty()) {
            cerr << "[StandardMSCSearch] Failed to load MCS filters.\n";
            return -1;
        }

        // Load search words
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[StandardMSCSearch] Failed to load search words.\n";
            return -1;
        }

        // Load filters map
        if (filters_map.empty()) {
            cerr << "[StandardMSCSearch] Failed to load filters map - its empty.\n";
            return -1;
        }

        // --- Prepare container to collect results ---
        vector<WordMatch> results;
        size_t total_words = search_words.size();

        // --- Iterate over each search word ---
        for (size_t widx = 0; widx < total_words; ++widx) {
            const string& word = search_words[widx];

            // --- Try all filters on this word ---
            for (const string& filter : filters) {
                size_t filter_length = filter.length();

                // --- Slide the filter over the word ---
                for (size_t i = 0; i + filter_length <= word.length(); ++i) {
                    string filtered_word;

                    // --- Apply filter to substring of word ---
                    for (size_t j = 0; j < filter_length; ++j) {
                        filtered_word += (filter[j] == '1') ? word[i + j] : '_';
                    }

                    // --- Check if this masked word appears in the filters map ---
                    auto it = filters_map.find(filtered_word);
                    if (it != filters_map.end()) {
                        // Add to results (avoid duplicates)
                        auto found = find_if(results.begin(), results.end(), [&](const WordMatch& m) {
                            return m.word == filtered_word;
                            });

                        if (found == results.end()) {
                            results.push_back(it->second);
                        }
                        else {
                            for (int pos : it->second.positions) {
                                if (find(found->positions.begin(), found->positions.end(), pos) == found->positions.end()) {
                                    found->positions.push_back(pos);
                                }
                            }
                        }
                    }
                }
            }
            print_progress(static_cast<int>(widx), static_cast<int>(total_words));
        }

        vector<string> output_lines = convert_matches_to_lines(results);

        // Save results to file
        int status = save_to_file(output_lines, STANDARD_MCS_OUTPUT_FILENAME);
        if (status == 0) cout << "[StandardMSCSearch] MSC search complete. Results saved to " << STANDARD_MCS_OUTPUT_FILENAME << '\n';
        return status;
    }
}