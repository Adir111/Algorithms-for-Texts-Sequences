#ifndef CREATE_FILTERS_MAP_HPP
#define CREATE_FILTERS_MAP_HPP

#include "utils.hpp"
#include "config.hpp"
#include "WordMatch.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace FiltersMap {
    extern std::unordered_map<std::string, WordMatch> filters_map;

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
     * @brief Creates a map of filtered words and their positions in the input text.
     *        The process involves reading text and filters, applying the filters over
     *        sliding windows in the text, building a map of filtered words to positions,
     *        and saving the result to a file.
     *
     * @return int Returns 0 on success, or -1 on failure.
     */
    int create_filters_map();
    }

#endif