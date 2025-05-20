#ifndef MSC_HPP
#define MSC_HPP

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
    vector<string> generate_valid_combinations(int total_bits, int required_ones);

    /**
     * @brief Generates a minimal set of binary strings (MSC) based on configuration parameters.
     *
     * @return int Returns 0 on success, -1 if saving the file failed.
     */
	int msc_creation();
}

#endif