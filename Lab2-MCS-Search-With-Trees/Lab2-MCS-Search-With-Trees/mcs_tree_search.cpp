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
    inline static bool check_matches(const string& text, size_t text_index, const string& word) {
        if (text_index > text.size() || text_index + word.length() > text.size()) {
            return false;
        }

        int matches = 0;
        for (size_t curr_word_index = 0; curr_word_index < word.length(); curr_word_index++) {
            if (text[text_index + curr_word_index] == word[curr_word_index]) {
                if (++matches >= MINIMAL_MATCHES) return true;
            } else {
                // Early exit if too few chars left to reach MINIMAL_MATCHES
                if ((word.length() - curr_word_index + matches) < MINIMAL_MATCHES) return false;
            }
        }
        return false;
    }

    /**
     * @brief Recursively collects positions of matches from the tree data structure.
     *
     * This function traverses the tree nodes to find matching positions for a given word
     * starting at a certain offset and depth. It collects the positions of matching
     * substrings and stores them in the output set.
     *
     * @param node The current node of the tree to explore.
     * @param word The word to search for in the tree.
     * @param offset The current offset within the word to match.
     * @param depth The current depth in the tree to traverse.
     * @param out The set to store the resulting matching positions.
     */
    static void collect_positions_from_tree(TreeNode* node, const string& word, size_t offset, size_t depth, unordered_set<size_t>& out) {
        if (!node || offset + depth >= word.length()) return;

        int idx = index_fixer(word[offset + depth]);
        int dollar_idx = index_fixer('$');

        // Check letter path
        if (node->pointers[idx]) {
            void* ptr = node->pointers[idx];
            if (node->is_leaf[idx]) {
                string* str_ptr = static_cast<string*>(ptr);
                auto it = tree_data.filters_map.find(*str_ptr);
                if (it != tree_data.filters_map.end()) {
                    out.insert(it->second.begin(), it->second.end());
                }
            }
            else {
                collect_positions_from_tree(static_cast<TreeNode*>(ptr), word, offset, depth + 1, out);
            }
        }

        // Check $ path
        if (node->pointers[dollar_idx]) {
            void* ptr = node->pointers[dollar_idx];
            if (node->is_leaf[dollar_idx]) {
                string* str_ptr = static_cast<string*>(ptr);
                auto it = tree_data.filters_map.find(*str_ptr);
                if (it != tree_data.filters_map.end()) {
                    out.insert(it->second.begin(), it->second.end());
                }
            }
            else {
                collect_positions_from_tree(static_cast<TreeNode*>(ptr), word, offset, depth + 1, out);
            }
        }
    }

    /**
     * @brief Retrieves all positions from the tree for a given word starting at a specific offset.
     *
     * This function calls `collect_positions_from_tree` to recursively gather matching
     * positions from the tree, starting at the provided offset for the given word.
     *
     * @param word The word to search for in the tree.
     * @param offset The offset within the word to match.
     * @param out The set to store the resulting matching positions.
     */
    inline static void get_all_positions_from_tree(const string& word, size_t offset, unordered_set<size_t>& out) {
        collect_positions_from_tree(tree_data.tree, word, offset, 0, out);
    }

    /**
     * @brief Executes the MCS Tree search algorithm.
     *
     * This function initiates the tree search by loading the text and search words,
     * initializing the tree data, and performing parallel searches for each word in the
     * search list. It collects matching positions, verifies matches against the text,
     * and stores the results. The results are then saved to an output file and a summary
     * of the search is saved.
     *
     * @return int Status code indicating success (0) or failure (-1).
     */
    int run_tree_mcs_search() {
        cout << "[MCSTreeSearch] Starting MCS Tree search...\n";

        // Init
        // === Load Text ===
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[MCSTreeSearch] Failed to load text - it's empty or doesn't exist.\n";
            return -1;
        }

        // === Load search words ===
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[MCSTreeSearch] Failed to load search words.\n";
            return -1;
        }

        int count_total_finds = 0;

        // === Phase 1: Verify tree exist and inits ===
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
        #pragma omp parallel for reduction(+:count_total_finds)
        for (size_t word_index = 0; word_index < total_words; ++word_index) {
            const string& word = search_words[word_index];
            const size_t word_length = word.length();
            
            // === Phase 3: Scan windows of word ===
            unordered_set<MatchPos> all_positions;

            for (size_t offset = 0; offset <= word_length - FILTER_NUMBER_OF_MATCHES; ++offset) {
                unordered_set<size_t> positions;
                get_all_positions_from_tree(word, offset, positions);

                for (size_t pos : positions) {
                    MatchPos mp = { pos - offset, offset };
                    all_positions.insert(mp);
                }
            }

            // Now iterate over all_positions
            for (const MatchPos& mp : all_positions) {
                if (mp.position + word.size() >= text.size()) continue;

                if (check_matches(text, mp.position, word)) {
                    if (insert_or_update_match(results, word, mp.position))
                        count_total_finds++;
                }
            }

            print_progress(static_cast<int>(word_index), static_cast<int>(total_words));
        }

        auto end = steady_clock::now();
        duration<double> elapsed_seconds = end - start;
        double seconds = elapsed_seconds.count();
        Summary summary = { "Tree Search", count_total_finds, seconds };

        vector<WordMatch> results_vector(results.begin(), results.end());
        vector<string> output_lines = convert_matches_to_lines(results_vector);

        // Save results to file
        int status = save_to_file(output_lines, MCS_TREE_SEARCH_OUTPUT_FILENAME, true);
        save_to_file(summary.to_lines(), MCS_TREE_SEARCH_SUMMARY_FILENAME, true);

        if (status == 0) cout << "[StandardMCSSearch] MCS search complete with total finds " << count_total_finds << ". Results saved to " << MCS_TREE_SEARCH_OUTPUT_FILENAME << '\n';
        return status;
	}
}
