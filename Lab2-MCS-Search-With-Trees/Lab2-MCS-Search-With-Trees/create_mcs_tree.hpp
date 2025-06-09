#ifndef CREATE_MCS_TREE_HPP
#define CREATE_MCS_TREE_HPP

#include "utils.hpp"
#include "config.hpp"
#include "TreeNode.hpp"

namespace Create_MCS_Tree {
	extern TreeNode* tree;

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