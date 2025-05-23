#ifndef NAIVE_SEARCH_HPP
#define NAIVE_SEARCH_HPP

#include <string>
#include <vector>
#include <set>
#include <chrono>

#include "config.hpp"
#include "utils.hpp"
#include "WordMatch.hpp"
#include "Summary.hpp"

namespace NaiveSearch {

    /**
     * @brief Performs naive search for all given words and writes results to file.
     *
     * @return int 0 on success, -1 on failure
     */
    int perform_naive_search();
}

#endif