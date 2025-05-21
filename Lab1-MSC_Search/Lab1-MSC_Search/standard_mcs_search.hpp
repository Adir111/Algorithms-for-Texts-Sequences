#ifndef STANDARD_MCS_SEARCH_HPP
#define STANDARD_MCS_SEARCH_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>

#include "WordMatch.hpp"
#include "utils.hpp"
#include "config.hpp"
#include "create_filters_map.hpp"

namespace StandardMCSSearch {

    /**
	 * @brief Applies a binary filter to a given word and returns a filtered version of the word.
	 *
	 * The method creates a filtered version of the input `word`, where characters corresponding
	 * to '1' positions in the filter are retained, and all others are replaced with underscores ('_').
	 *
	 * @param word The word to be filtered.
	 * @param filter A string of '0's and '1's representing the filter. '1' means the character at that position
	 *               will be kept from the original word, and '0' means it will be replaced with '_'.
	 * @param sliding_window_index The starting index within the word to begin applying the filter.
	 *
	 * @return A filtered string where the characters are either from the `word` or '_' depending on the filter.
	 */
	string applyFilterToWord(const string& word, const string& filter, size_t sliding_window_index);

	/**
	 * @brief Searches for matches of a filtered word in a given text and returns the total count of matches found.
	 *
	 * This method searches for occurrences of `filtered_word` in the global `filters_map` and compares the characters
	 * in surrounding positions of the `text` to count the matches. If the number of matches exceeds the defined threshold,
	 * it updates the `results` vector with the matching positions and increments the total count of finds.
	 *
	 * @param filtered_word The word after applying the filter, which is to be searched.
	 * @param word The original word being checked for matches.
	 * @param text The text to search for matches in.
	 * @param sliding_window_index The index used for adjusting word positions in the text.
	 * @param results A set to store the matches.
	 * @return The total count of finds (matches found).
	 */
	int searchAndInsertMatches(const string& filtered_word, const string& word, const string& text,
		size_t sliding_window_index, set<WordMatch>& results);

	/**
	 * @brief Executes a standard MCS search using filters and a filters map.
	 *
	 * The method processes each search word and applies all MCS filters by sliding them across the word.
	 * For every valid application of a filter, it generates a masked word (with '_' in unmatched positions),
	 * looks it up in the filters map, and collects matching positions.
	 * Finally, the results are saved to the file defined in the config under STANDARD_MCS_OUTPUT_FILENAME.
	 *
	 * @return 0 on success, -1 on failure (e.g., failed to load files or save output)
	 */
	int run_standard_mcs_search();
}

#endif