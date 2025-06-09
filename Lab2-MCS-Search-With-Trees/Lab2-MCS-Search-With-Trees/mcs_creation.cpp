#include "mcs_creation.hpp"

using namespace std;
using namespace Config;
using namespace Utils;

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
    vector<string> generate_valid_combinations() {
        const int total_bits = SEARCH_WORD_SIZE;
        const int required_ones = MINIMAL_MATCHES;

        vector<string> combinations;
        const unsigned long long start = 1ULL << (total_bits - 1);  // 100...0
        const unsigned long long end = (1ULL << total_bits);        // one past 111...1

        const int total_range = static_cast<int>(end - start);
        int generated_count = 0;

        for (unsigned long long i = end - 1; i >= start; --i) {
            bitset<SEARCH_WORD_SIZE> bits(i);  // Convert to fixed-size binary string
            string binary = bits.to_string();
            if (count(binary.begin(), binary.end(), '1') == required_ones) {
                combinations.push_back(binary);
            }

            ++generated_count;
            print_progress(generated_count, total_range);

            if (i == start) break;  // avoid infinite loop on unsigned underflow
        }

        return combinations;
    }

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
    bool is_filter_in_combination(const string& filter, const string& combination) {
        for (size_t i = 0; i <= combination.size() - filter.size(); ++i) {
            if (filter[0] == combination[i]) {
                bool match = true;
                for (size_t j = 1; j < filter.size(); j++) {
                    if (filter[j] == 0)
                        continue;
                    if (filter[j] != combination[i + j]) {
                        match = false;
                        break;
                    }
                }
                if (match) return true;
            }
        }
        return false;
    }

    /**
     * @brief Generates a minimal set of binary strings (MCS) based on configuration parameters.
     *
     * @return int Returns 0 on success, -1 if saving the file failed.
     *
     * The function follows these steps:
     * 1. Generates all binary strings of size SEARCH_WORD_SIZE that:
     *    - Start with '1'
     *    - Contain exactly MINIMAL_MATCHES number of '1's
     * 2. Filters and processes these strings into a minimal set (MCS) by:
     *    - Skipping strings that already contain an existing MCS entry as a substring
     *    - Truncating strings after FILTER_AMOUNT_OF_MATCHES ones
     *    - Avoiding duplicates in the MCS
     * 3. Saves the MCS to a file defined in MCS_OUTPUT_FILENAME
     */
    int mcs_creation() {
        cout << "[MCS] Starting MCS creation process...\n";
        vector<string> mcs;

        // === Step 1: Generate all combinations with exact number of ones ===
        cout << "[MCS] Generating binary strings with " << MINIMAL_MATCHES << " ones...\n";
        vector<string> all_valid_combinations = generate_valid_combinations();

        const int total_combinations = static_cast<int>(all_valid_combinations.size());
        cout << "[MCS] " << total_combinations << " valid combinations generated.\n";
        cout << "[MCS] Creating minimal set cover...\n";

        // === Step 2: Create the minimal set cover (MCS) ===
        int processed = 0;

        for (const auto& value : all_valid_combinations) {
            bool skip = false;

            // Skip if current value contains an existing MCS entry as substring
            for (const auto& existing : mcs) {
                skip = is_filter_in_combination(existing, value);
                if (skip) break;
            }
            if (skip) {
                ++processed;
                print_progress(processed, total_combinations);
                continue;
            }

            // Truncate after FILTER_NUMBER_OF_MATCHES ones
            string truncated;
            int ones_seen = 0;
            for (char ch : value) {
                truncated += ch;
                if (ch == '1') {
                    ++ones_seen;
                }
                if (ones_seen == FILTER_NUMBER_OF_MATCHES) {
                    break;
                }
            }

            // Add to MCS if not already present
            if (find(mcs.begin(), mcs.end(), truncated) == mcs.end()) {
                mcs.push_back(truncated);
            }

            ++processed;
            print_progress(processed, total_combinations);
        }

        // === Step 3: Save result to output file ===
        int status = save_to_file(mcs, MCS_OUTPUT_FILENAME);
        if (status == 0) cout << "\n[MCS] MCS created with " << mcs.size() << " entries.\n";
        return status;
    }
}
