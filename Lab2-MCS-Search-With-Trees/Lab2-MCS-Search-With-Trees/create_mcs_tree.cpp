#include "create_mcs_tree.hpp"

using namespace std;
using namespace Utils;
using namespace Config;

namespace Create_MCS_Tree {

    /**
     * @brief Applies a binary filter to a given word and returns a filtered version of the word.
     *
     * The method creates a filtered version of the input `word`, where characters corresponding
     * to '1' positions in the filter are retained, and all others are replaced with dollar signs ('$').
     *
     * @param word The word to be filtered.
     * @param filter A string of '0's and '1's representing the filter. '1' means the character at that position
     *               will be kept from the original word, and '0' means it will be replaced with '$'.
     *
     * @return A filtered string where the characters are either from the `word` or '$' depending on the filter.
     */
    static string apply_filter_to_word(const string& word, const string& filter) {
        string filtered_word(word.size(), '$');

        for (size_t i = 0; i < filter.size(); ++i)
            if (filter[i] == '1')
                filtered_word[i] = word[i];

        return filtered_word;
    }

    /**
     * @brief Maps a character to an index in the pointer array.
     * @param letter The input character, where '$' is treated specially.
     * @return Integer index corresponding to the character.
     */
    int index_fixer(char letter) {
        if (letter == '$')
            return SIZE - 1;
        return letter - Y_LETTER;
    }

    /**
     * @brief Constructs a multi-filter character sequence tree from input files.
     *
     * Reads a random text file and a set of MCS filters, then builds a tree
     * by sliding a window over the text and inserting filtered characters.
     *
     * @return 0 on success, -1 on failure (e.g., file loading error).
     */
    int create_mcs_tree() {
        // === Phase 1: Read files ===
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[MCSTreeBuilder] Failed to load text.\n";
            return -1;
        }

        vector<string> filters = read_lines_from_file(MCS_OUTPUT_FILENAME);
        if (filters.empty()) {
            cerr << "[MCSTreeBuilder] Failed to load MCS filters.\n";
            return -1;
        }

        // Init
        tree_data.tree = new TreeNode();
        size_t text_len = text.length();
        size_t total_iterations = text_len - SEARCH_WORD_SIZE + 1;

        // === Phase 2: Iterate through text ===
        for (size_t i = 0; i < total_iterations; ++i) {

            // === Phase 3: Iterate through filters ===
            for (const string& filter : filters) {
                string word = text.substr(i, filter.size());

                string filtered = apply_filter_to_word(word, filter);
                tree_data.filters_map[filtered].insert(i);

                // === Phase 4: Create Tree Nodes
                TreeNode* current = tree_data.tree;
                for (size_t j = 0; j < filtered.size(); ++j) {
                    char ch = filtered[j];
                    int idx = index_fixer(ch);

                    if (j != filtered.size() - 1) {
                        if (current->pointers[idx] == nullptr) {
                            TreeNode* new_node = new TreeNode(current);
                            current->pointers[idx] = static_cast<void*>(new_node);
                        }
                        current = static_cast<TreeNode*>(current->pointers[idx]);
                    }
                    else {
                        // Last character — store filtered word instead of creating a new node
                        if (current->pointers[idx] == nullptr) {
                            current->pointers[idx] = static_cast<void*>(new string(filtered));
                            current->is_leaf[idx] = true;
                        }
                    }

                }
            }

            print_progress(static_cast<int>(i + 1), static_cast<int>(total_iterations));
        }

        cout << "[MCSTreeBuilder] MCS tree creation complete.\n";
        return 0;
    }  
}