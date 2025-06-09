#include "create_mcs_tree.hpp"

using namespace std;
using namespace Utils;
using namespace Config;

namespace Create_MCS_Tree {
    /// Global tree root pointer
    TreeNode* tree = nullptr;

    /**
     * @brief Maps a character to an index in the pointer array.
     * @param letter The input character, where '$' is treated specially.
     * @return Integer index corresponding to the character.
     */
    static int index_fixer(char letter) {
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
        if (tree != nullptr) {
            cout << "[MCSTreeBuilder] Tree has already been built!";
            return 0;
        }

        cout << "[MCSTreeBuilder] Starting filters map creation...\n";

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

        // === Phase 2: Iterate through text ===
        tree = new TreeNode();
        size_t text_len = text.length();
        size_t total_iterations = text_len - SEARCH_WORD_SIZE + 1;

        for (size_t i = 0; i < total_iterations; ++i) {
            for (const string& filter : filters) {
                TreeNode* current = tree;

                for (size_t j = 0; j < filter.size(); ++j) {
                    char ch = (filter[j] == '1') ? text[i + j] : '$';
                    int idx = index_fixer(ch);

                    if (current->pointers[idx] == nullptr) {
                        TreeNode* new_node = new TreeNode(current);
                        current->pointers[idx] = static_cast<void*>(new_node);
                    }

                    current = static_cast<TreeNode*>(current->pointers[idx]);
                }
            }

            print_progress(static_cast<int>(i + 1), static_cast<int>(total_iterations));
        }

		return 0;
	}
}