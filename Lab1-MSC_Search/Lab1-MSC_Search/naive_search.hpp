#ifndef NAIVE_SEARCH_HPP
#define NAIVE_SEARCH_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "config.hpp"
#include "utils.hpp"
#include "WordMatch.hpp"

namespace NaiveSearch {

    /**
     * @brief Performs naive search for all given words and writes results to file.
     *
     * @param search_words Vector of words to search
     * @param output_filename Output file name
     * @return int 0 on success, -1 on failure
     */
    int perform_naive_search();
    }

#endif