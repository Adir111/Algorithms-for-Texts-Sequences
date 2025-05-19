#ifndef STANDARD_MCS_SEARCH_HPP
#define STANDARD_MCS_SEARCH_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

#include "WordMatch.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "create_filters_map.hpp"

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
	int run_standard_msc_search();
}

#endif