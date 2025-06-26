#include "create_mcs_tree.hpp"

using namespace std;
using namespace Utils;
using namespace Config;

namespace Create_MCS_Tree {

    /**
     * @brief Returns the index of the word in the filters_map. If it doesn't exist, adds it.
     *
     * @param word The filtered word.
     * @param position The position in the text to insert.
     * @return Index of the WordMatch in the filters_map vector.
     */
    static size_t get_or_add_filtered_word(const std::string& word, size_t position) {
        for (size_t i = 0; i < tree_data_array.filters_positions.size(); ++i) {
            if (tree_data_array.filters_positions[i].word == word) {
                tree_data_array.filters_positions[i].positions.insert(position);
                return i;
            }
        }

        // If not found, create and add a new WordMatch
        WordMatch new_match;
        new_match.word = word;
        new_match.positions.insert(position);
        tree_data_array.filters_positions.push_back(new_match);
        return tree_data_array.filters_positions.size() - 1;
    }

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

    static int convert_node_tree_to_array() {
        if (!tree_data.tree) {
            cerr << "[MCSTreeBuilder] Cannot convert: tree_data.tree is null.\n";
            return -1;
        }

        // Reset the array-based data
        tree_data_array.tree.clear();
        tree_data_array.filters_positions.clear();

        // === Step 1: Copy filters_map to filters_positions ===
        for (const auto& pair : tree_data.filters_map) {
            WordMatch match;
            match.word = pair.first;
            match.positions = pair.second;
            tree_data_array.filters_positions.push_back(std::move(match));
        }

        // === Step 2: Map from TreeNode* to array index ===
        std::unordered_map<TreeNode*, int> node_to_index;
        std::queue<std::pair<TreeNode*, int>> q;

        tree_data_array.tree.emplace_back(); // root node
        node_to_index[tree_data.tree] = 0;
        q.push({ tree_data.tree, 0 });

        size_t processed_nodes = 0;
        size_t total_estimated_nodes = tree_data.filters_map.size();

        while (!q.empty()) {
            TreeNode* current_node = q.front().first;
            int current_index = q.front().second;
            q.pop();

            for (int i = 0; i < Config::SIZE; ++i) {
                if (current_node->pointers[i] == nullptr)
                    continue;

                if (current_node->is_leaf[i]) {
                    // Leaf: pointer holds a string*
                    string* filtered_word = static_cast<string*>(current_node->pointers[i]);

                    // Find index of the filtered word in filters_positions
                    auto it = std::find_if(
                        tree_data_array.filters_positions.begin(),
                        tree_data_array.filters_positions.end(),
                        [&](const WordMatch& wm) { return wm.word == *filtered_word; });

                    if (it == tree_data_array.filters_positions.end()) {
                        cerr << "[MCSTreeBuilder] Warning: Filtered word not found: " << *filtered_word << "\n";
                        continue;
                    }

                    int leaf_index = static_cast<int>(std::distance(tree_data_array.filters_positions.begin(), it));
                    tree_data_array.tree[current_index].pointers[i] = leaf_index;
                    tree_data_array.tree[current_index].is_leaf[i] = true;
                }
                else {
                    // Internal node: pointer holds a TreeNode*
                    TreeNode* child_node = static_cast<TreeNode*>(current_node->pointers[i]);

                    // If not visited, assign new index and push to queue
                    if (node_to_index.find(child_node) == node_to_index.end()) {
                        int new_index = static_cast<int>(tree_data_array.tree.size());
                        node_to_index[child_node] = new_index;
                        tree_data_array.tree.emplace_back();
                        q.push({ child_node, new_index });
                    }

                    int child_index = node_to_index[child_node];
                    tree_data_array.tree[current_index].pointers[i] = child_index;
                }
            }

            processed_nodes++;
            print_progress(static_cast<int>(processed_nodes), static_cast<int>(total_estimated_nodes));
        }

        cout << "[MCSTreeBuilder] Successfully converted node tree to array representation.\n";
        return 0;
    }


    // Tree as nodes with addresses implementation
    static int nodes_addresses_create_mcs_tree(const string& text, const vector<string>& filters) {
        tree_data.tree = new TreeNode();
        size_t text_len = text.length();
        size_t total_iterations = text_len - SEARCH_WORD_SIZE + 1;

        // === Phase 1: Iterate through text ===
        for (size_t i = 0; i < total_iterations; ++i) {

            // === Phase 2: Iterate through filters ===
            for (const string& filter : filters) {
                string word = text.substr(i, filter.size());

                string filtered = apply_filter_to_word(word, filter);
                tree_data.filters_map[filtered].insert(i);

                // === Phase 3: Create Tree Nodes
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

    /**
     * @brief Constructs a multi-filter character sequence tree from input files.
     *
     * Reads a random text file and a set of MCS filters, then builds a tree
     * by sliding a window over the text and inserting filtered characters.
     * 
     * Implementing tree as array and also nodes with addresses.
     *
     * @return 0 on success, -1 on failure (e.g., file loading error).
     */
    int create_mcs_tree() {
        if (tree_data.tree != nullptr || tree_data_array.tree.size() != 0) {
            cout << "[MCSTreeBuilder] Tree has already been built!\n";
            return 0;
        }

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

        cout << "[MCSTreeBuilder] Starting creating MCS tree as nodes with addresses.\n";
        if (nodes_addresses_create_mcs_tree(text, filters) != 0) {
            cout << "[MCSTreeBuilder] Failed creating MCS tree as nodes with addresses!\n";
            return -1;
        }
        cout << "[MCSTreeBuilder] Converting MCS tree to array representation.\n";
        return convert_node_tree_to_array();
    }

    
}