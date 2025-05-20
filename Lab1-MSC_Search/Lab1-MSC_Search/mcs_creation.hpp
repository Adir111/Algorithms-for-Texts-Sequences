#ifndef MCS_HPP
#define MCS_HPP

#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
#include <unordered_set>

#include "config.hpp"
#include "utils.hpp"

namespace MCS {

    /**
     * @brief Generates all valid binary strings of given bit length
     *        that start with '1' and contain exactly `required_ones` number of '1's.
     *
     * @param total_bits The total number of bits in each binary string.
     * @param required_ones The exact number of '1's each binary string must contain.
     * @return std::vector<std::string> A vector of valid binary strings.
     *
     * The function avoids unsigned underflow and shows progress while generating.
     */
    vector<string> generate_valid_combinations();

    /**
     * @brief Checks whether a filter exists as a substring in a given combination.
     *
     * This function looks for an occurrence of `filter` within `combination`, treating
     * filter positions with null characters (0) as wildcards that match any character.
     *
     * @param filter The binary filter string (may include null characters as wildcards).
     * @param combination The binary string to check for inclusion of the filter.
     * @return true if the filter is found in the combination, false otherwise.
     */
    bool is_filter_in_combination(const string& filter, const string& combination);

    /**
     * @brief Generates a minimal set of binary strings (MCS) based on configuration parameters.
     *
     * @return int Returns 0 on success, -1 if saving the file failed.
     */
	int mcs_creation();
}

#endif