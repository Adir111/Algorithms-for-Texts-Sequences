#include "mcs_tree_search.hpp"

using namespace std;
using namespace chrono;
using namespace Utils;
using namespace Config;
using namespace Create_MCS_Tree;

namespace Tree_MCS_Search {

    /**
     * @brief Checks if a substring of the given text matches the target word
     *        with at least a minimum number of character matches.
     *
     * This function compares the substring of `text` starting at `text_index`
     * with the `word`, character by character. It counts how many characters
     * match at the corresponding positions. If the number of matching characters
     * is greater than or equal to `MINIMAL_MATCHES`, the function returns true.
     *
     * @param text The full text in which to search for a partial match.
     * @param text_index The starting index in the text where the comparison begins.
     * @param word The word to compare against the substring of the text.
     * @return true if the number of matching characters is >= MINIMAL_MATCHES;
     *         false otherwise.
     *
     */
    static bool check_matches(const string& text, size_t text_index, const string& word) {
        if (text_index > text.size() || text_index + word.length() > text.size()) {
            return false;
        }

        int matches = 0;
        for (size_t curr_word_index = 0; curr_word_index < word.length(); curr_word_index++) {
            if (text[text_index + curr_word_index] == word[curr_word_index])
                matches++;
        }
        return matches >= MINIMAL_MATCHES;
    }

    const set<size_t>* get_positions_from_tree(const string& word, size_t offset) {
        TreeNode* current = tree_data.tree;

        for (size_t i = 0; i < FILTER_NUMBER_OF_MATCHES; ++i) {
            int idx = index_fixer(word[offset + i]);

            // Try regular path first
            if (!current || !current->pointers[idx]) {
                // Fallback to '$' path
                int dollar_idx = index_fixer('$');
                if (!current || !current->pointers[dollar_idx]) {
                    return nullptr; // neither path exists
                }
                idx = dollar_idx;
            }

            if (i == FILTER_NUMBER_OF_MATCHES - 1) {
                // Last character - check if this is a string* leaf
                void* ptr = current->pointers[idx];
                string* str_ptr = static_cast<string*>(ptr);

                // Confirm it's a string by checking if the filters_map contains it
                auto it = tree_data.filters_map.find(*str_ptr);
                if (it != tree_data.filters_map.end()) {
                    return &it->second;
                }
                else {
                    return nullptr;
                }
            }

            // Advance to next TreeNode
            current = static_cast<TreeNode*>(current->pointers[idx]);
        }

        return nullptr; // fallback (shouldn't reach here)
    }

	int run_tree_mcs_search() {
        cout << "[MCSTreeSearch] Starting MCS Tree search...\n";
        int count_total_finds = 0;

        // === Phase 1: Read files ===
        // Load Text
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[MCSTreeSearch] Failed to load text - its empty or doesn`t exist.\n";
            return -1;
        }

        // Load search words
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[MCSTreeSearch] Failed to load search words.\n";
            return -1;
        }

        // Verify tree exist
        if (tree_data.tree == nullptr) {
            cerr << "[MCSTreeSearch] Failed to load tree data - it doesnt exist.\n";
            return -1;
        }


        // --- Prepare container to collect results ---
        set<WordMatch> results;
        size_t total_words = search_words.size();

        auto start = steady_clock::now();

        // === Phase 2: Iterate over each search word ===
        cout << "[MCSTreeSearch] Start iterating over search words...\n";
        for (size_t word_index = 0; word_index < total_words; ++word_index) {
            const string& word = search_words[word_index];
            const size_t word_length = word.length();
            
            // === Phase 3: Scan windows of word ===
            for (size_t offset = 0; offset <= word_length - FILTER_NUMBER_OF_MATCHES; ++offset) {
                const set<size_t>* positions = get_positions_from_tree(word, offset);
                if (positions) {
                    for (size_t pos : *positions) {
                        size_t position_text = pos - offset;

                        if (position_text + word.size() >= text.size())
                            continue;

                        if (check_matches(text, position_text, word)) {
                            if (insert_or_update_match(results, word, position_text))
                                count_total_finds++;
                        }
                    }
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
        int status = save_to_file(output_lines, MCS_TREE_SEARCH_OUTPUT_FILENAME, true);
        save_to_file(summary.to_lines(), MCS_TREE_SEARCH_SUMMARY_FILENAME, true);

        if (status == 0) cout << "[StandardMCSSearch] MCS search complete with total finds " << count_total_finds << ". Results saved to " << MCS_TREE_SEARCH_OUTPUT_FILENAME << '\n';
        return status;
	}
}
