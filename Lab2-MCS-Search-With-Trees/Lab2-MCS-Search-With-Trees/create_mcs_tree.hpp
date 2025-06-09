#ifndef CREATE_MCS_TREE_HPP
#define CREATE_MCS_TREE_HPP

#include "utils.hpp"
#include "config.hpp"
#include "TreeNode.hpp"
#include "TreeData.hpp"

namespace Create_MCS_Tree {

    /**
     * @brief Maps a character to an index in the pointer array.
     * @param letter The input character, where '$' is treated specially.
     * @return Integer index corresponding to the character.
     */
    int index_fixer(char letter);

    /**
     * @brief Constructs a multi-filter character sequence tree from input files.
     *
     * Reads a random text file and a set of MCS filters, then builds a tree
     * by sliding a window over the text and inserting filtered characters.
     *
     * @return 0 on success, -1 on failure (e.g., file loading error).
     */
    int create_mcs_tree();
}

#endif
