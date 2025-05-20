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
     * @brief Generates a minimal set of binary strings (MSC) based on configuration parameters.
     *
     * @return int Returns 0 on success, -1 if saving the file failed.
     */
	int msc_creation();
}

#endif