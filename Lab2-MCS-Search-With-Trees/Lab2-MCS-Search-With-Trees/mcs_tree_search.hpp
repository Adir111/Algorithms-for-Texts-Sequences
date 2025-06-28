#ifndef TREE_MCS_SEARCH_HPP
#define TREE_MCS_SEARCH_HPP

#include "utils.hpp"
#include "config.hpp"
#include "TreeNode.hpp"
#include "TreeData.hpp"
#include "create_mcs_tree.hpp"
#include <unordered_map>
#include <unordered_set>
#include "Summary.hpp"
#include <chrono>
#include "MatchPos.hpp"

namespace Tree_MCS_Search {
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
	int run_tree_mcs_search();
}

#endif