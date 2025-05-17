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