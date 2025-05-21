#ifndef POSITIONAL_MCS_SEARCH_HPP
#define POSITIONAL_MCS_SEARCH_HPP



#include "WordMatch.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "create_filters_map.hpp"
#include "standard_mcs_search.hpp"

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
	int run_positional_mcs_search();
}

#endif