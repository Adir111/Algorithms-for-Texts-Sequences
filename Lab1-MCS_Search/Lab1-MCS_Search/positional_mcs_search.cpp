#include "positional_mcs_search.hpp"

using namespace std;
using namespace Utils;
using namespace Config;
using namespace FiltersMap;
using namespace StandardMCSSearch;
using namespace chrono;

namespace PositionalMCSSearch {

    /**
     * @brief Executes a positional MCS (Masked Common Substring) search on a given text using precomputed filters.
     *
     * This function loads the generated text, search words, standard MCS filters,
     * and multiple sets of positional MCS filters. It then applies these filters
     * over all sliding windows in the search words and checks for matches in a
     * precomputed filters map. The results, along with their matched positions,
     * are collected and saved to a result file.
     *
     * The function assumes all input files (text, filters, search words, filters map)
     * already exist and are properly formatted. It also assumes the filters map has
     * been previously generated and is globally available.
     *
     * @return int Returns 0 on success, or -1 on any failure during input loading or saving.
     */
    int run_positional_mcs_search() {
        cout << "[PositionalMCSSearch] Starting usual MCS search...\n";

        // Load Text
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[PositionalMCSSearch] Failed to load text - its empty or doesn`t exist.\n";
            return -1;
        }

        // Load MCS filters
        vector<vector<string>> all_mcs;
        int i = 0;
        while (i >= 0) {
            string filename = POSITIONAL_MCS_OUTPUT_FILENAME + "_" + to_string(i);
            vector<string> curr_mcs_filters = read_lines_from_file(filename, false);
            if (curr_mcs_filters.empty()) {
                if (i == 0) {
                    cerr << "[PositionalMCSSearch] Failed to load MCS filters - its empty or doens`t exist.\n";
                    return -1;
                }
                break;
            }
            all_mcs.push_back(curr_mcs_filters);
            i++;
        }
        cout << "[PositionalMCSSearch] Successfully loaded all MCS files.\n";

        // Load search words
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[PositionalMCSSearch] Failed to load search words.\n";
            return -1;
        }

        // Verify filters map exist
        if (filters_map.empty()) {
            cerr << "[PositionalMCSSearch] Failed to load filters map - its empty.\n";
            return -1;
        }

        // --- Prepare container to collect results ---
        set<WordMatch> results;
        size_t total_words = search_words.size();
        size_t total_mcs = all_mcs.size();
        int count_total_finds = 0;

        auto start = steady_clock::now();

        // --- Iterate over each search word ---
        cout << "[PositionalMCSSearch] Start iterating over search words..\n";
        for (size_t word_index = 0; word_index < total_words; ++word_index) {
            const string& word = search_words[word_index];
            const size_t search_word_length = word.length();

            // --- Iterate over each all_mcs[i]
            for (size_t mcs_filter_index = 0; mcs_filter_index < total_mcs; mcs_filter_index++) { // mcs_filter_index is also the sliding window index

                // --- Try all filters on this word ---
                for (const string& filter : all_mcs[mcs_filter_index]) {
                    string filtered_word = apply_filter_to_word(word, filter, mcs_filter_index);

                    // --- Check if this masked word appears in the filters map ---
                    count_total_finds += search_and_insert_matches(filtered_word, word, text, mcs_filter_index, results);
                }
            }
            print_progress(static_cast<int>(word_index), static_cast<int>(total_words));
        }

        auto end = steady_clock::now();
        duration<double> elapsed_seconds = end - start;
        double seconds = elapsed_seconds.count();
        Summary summary = { "Positional Search", count_total_finds, seconds };

        vector<WordMatch> results_vector(results.begin(), results.end());
        vector<string> output_lines = convert_matches_to_lines(results_vector);

        // Save results to file
        int status = save_to_file(output_lines, POSITIONAL_MCS_SEARCH_OUTPUT_FILENAME, true);
        if (save_to_file(summary.to_lines(), POSITIONAL_MCS_SEARCH_SUMMARY_FILENAME, true) != 0)
            cout << "[PositionalMCSSearch] Summary failed saving.\n";
        else cout << "[PositionalMCSSearch] Summary file has saved to " << POSITIONAL_MCS_SEARCH_SUMMARY_FILENAME << "\n";

        if (status == 0) cout << "[PositionalMCSSearch] MCS search complete with total finds " << count_total_finds << ". Results saved to " << STANDARD_MCS_OUTPUT_FILENAME << '\n';
        return status;
    }
}