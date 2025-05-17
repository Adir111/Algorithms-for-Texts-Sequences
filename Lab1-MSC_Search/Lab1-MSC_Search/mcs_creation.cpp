#include "mcs_creation.hpp"

using namespace Config;
using namespace Utils;
using namespace std;

namespace MCS {

    /**
     * @brief Generates a minimal set of binary strings (MSC) based on configuration parameters.
     *
     * @return int Returns 0 on success, -1 if saving the file failed.
     * 
     * The function follows these steps:
     * 1. Generates all binary strings of size SEARCH_WORD_SIZE that:
     *    - Start with '1'
     *    - Contain exactly MINIMAL_MATCHES number of '1's
     * 2. Filters and processes these strings into a minimal set (MSC) by:
     *    - Skipping strings that already contain an existing MSC entry as a substring
     *    - Truncating strings after FILTER_AMOUNT_OF_MATCHES ones
     *    - Avoiding duplicates in the MSC
     * 3. Saves the MSC to a file defined in MSC_OUTPUT_FILENAME
     */
    int msc_creation() {
        cout << "[MSC] Starting MSC creation process...\n";
        vector<string> all_valid_combinations;
        vector<string> msc;

        const int total_bits = SEARCH_WORD_SIZE;
        const int required_ones = MINIMAL_MATCHES;

        // Range: binary strings starting with 1 (e.g., 100...0) up to (but not including) the next power of 2
        const unsigned long long start = 1ULL << (total_bits - 1);  // 100...0
        const unsigned long long end = (1ULL << total_bits);      // one past 111...1

        cout << "[MSC] Generating binary strings with " << required_ones << " ones...\n";

        const int total_range = static_cast<int>(end - start);
        int generated_count = 0;

        // === Step 1: Generate all combinations with required number of 1 ===
        for (unsigned long long i = start; i < end; ++i) {
            bitset<SEARCH_WORD_SIZE> bits(i);  // Convert to fixed-size binary string
            string binary = bits.to_string();  // Get string representation
            if (count(binary.begin(), binary.end(), '1') == required_ones) {
                all_valid_combinations.push_back(binary);
            }

            ++generated_count;
            print_progress(generated_count, total_range);
        }

        cout << "[MSC] " << all_valid_combinations.size() << " valid combinations generated.\n";
        cout << "[MSC] Creating minimal set cover...\n";

        // === Step 2: Create the minimal set cover (MSC) ===
        const int total_combinations = static_cast<int>(all_valid_combinations.size());
        int processed = 0;

        for (const auto& value : all_valid_combinations) {
            bool skip = false;

            // Skip if current value contains an existing MSC entry as substring
            for (const auto& existing : msc) {
                if (value.find(existing) != string::npos) {
                    skip = true;
                    break;
                }
            }
            if (skip) {
                ++processed;
                print_progress(processed, total_combinations);
                continue;
            }

            // Truncate after FILTER_AMOUNT_OF_MATCHES ones
            string truncated;
            int ones_seen = 0;
            for (char ch : value) {
                truncated += ch;
                if (ch == '1') {
                    ++ones_seen;
                }
                if (ones_seen == FILTER_AMOUNT_OF_MATCHES) {
                    break;
                }
            }

            // Add to MSC if not already present
            if (find(msc.begin(), msc.end(), truncated) == msc.end()) {
                msc.push_back(truncated);
            }

            ++processed;
            print_progress(processed, total_combinations);
        }

        cout << "\n[MSC] MSC created with " << msc.size() << " entries.\n";

        // === Step 3: Save result to output file ===
        return save_to_file(msc, MSC_OUTPUT_FILENAME);
    }
}
