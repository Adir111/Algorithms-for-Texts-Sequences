#include "positional_mcs_creation.hpp"

using namespace std;
using namespace Config;
using namespace Utils;
using namespace MCS;

namespace PositionalMCS {

    /**
     * @brief Retrieves the initial MCS (mcs[0]) from file. If the file is empty, it triggers its creation.
     *
     * @return vector<string> A vector of binary filters representing the first minimal set cover.
     */
    static vector<string> get_mcs0() {
        vector<string> filters = read_lines_from_file(STANDARD_MCS_OUTPUT_FILENAME);
        if (filters.empty()) {
            if (mcs_creation() == -1) {
                cout << "[PositionalMCS] Failed mcs[0] creation.\n";
                return {};
            }
            return read_lines_from_file(STANDARD_MCS_OUTPUT_FILENAME);
        }
        return filters;
    }

    /**
     * @brief Checks whether a valid combination should be removed based on a given MCS filter.
     *
     * A combination should be removed if it covers all the bits marked as '1' in the filter.
     *
     * @param mcs_filter The binary filter to check against.
     * @param combination The binary combination to evaluate.
     * @return true if the combination should be removed, false otherwise.
     */
    static bool should_remove_combination(const string& mcs_filter, const string& combination, int position) {
        if (position + mcs_filter.size() > combination.size()) {
            cout << "[PositionalMCS] Invalid filter or position range. Skipping...\n";
            return false;
        }

        for (size_t i = 0; i < mcs_filter.size(); ++i) {
            if (mcs_filter[i] == '1' && combination[i + position] != '1') {
                return false;
            }
        }
        return true;
    }

	int positional_mcs_creation() {
        cout << "[PositionalMCS] Starting Positional MCS creation process...\n";
        vector<vector<string>> all_mcs;
        vector<string> mcs0 = get_mcs0();
        if (mcs0.empty()) {
            cout << "[PositionalMCS] Failed to get mcs[0], aborting process.\n";
            return -1;
        }
        all_mcs.push_back(mcs0);

        // === Step 1: Generate all combinations with exact number of ones ===
        cout << "[PositionalMCS] Generating binary strings with " << MINIMAL_MATCHES << " ones...\n";
        vector<string> all_valid_combinations = generate_valid_combinations();

        cout << "[PositionalMCS] " << static_cast<int>(all_valid_combinations.size()) << " valid combinations generated.\n";
        cout << "[PositionalMCS] Creating minimal set cover...\n";

        // === Step 2: Generate MCS for every position (for i + 1 using mcs[i])
        int mcs_index = 0;
        vector<string> mcs_i_plus_1;

        while (all_valid_combinations.size() > 0 && (mcs_index + FILTER_AMOUNT_OF_MATCHES) <= MINIMAL_MATCHES) { // stop when all combinations are removed.
            cout << "[PositionalMCS] Started process for MCS[" << mcs_index + 1 << "]..\n";
            int processed = 0;
            mcs_i_plus_1 = all_mcs[mcs_index];

            // == Phase 2.1: Reduction of combinations
            cout << "[PositionalMCS] Reduction of combinations phase has started.\n";
            int total_combinations = static_cast<int>(all_valid_combinations.size());
            vector<string> reduced_combinations;

            for (int combination_index = total_combinations - 1; combination_index >= 0; --combination_index) {
                bool should_remove = false;

                // Check if current combination is already covered by any filter in mcs[i]
                for (const string& filter : mcs_i_plus_1) {
                    should_remove = should_remove_combination(filter, all_valid_combinations[combination_index], mcs_index);
                    if (should_remove) break;
                }

                // Keep only combinations that doesnt have a filter at the start of them
                if (!should_remove)
                    reduced_combinations.push_back(all_valid_combinations[combination_index]);

                print_progress(processed++, total_combinations);
            }

            all_valid_combinations = reduced_combinations;

            // == Phase 2.2: Indicate protected filters
            bool should_continue = true;
            while (should_continue) {
                cout << "\n[PositionalMCS] Identifying protected filters phase has started.\n";
                int total_reduced = static_cast<int>(reduced_combinations.size());
                vector<bool> mcs_filter_protected(mcs_i_plus_1.size(), true);
                processed = 0;

                for (const string& combination : reduced_combinations) {
                    vector<bool> mcs_filter_match(mcs_i_plus_1.size(), false);
                    for (size_t j = 0; j < mcs_i_plus_1.size(); ++j) {
                        const string& filter = mcs_i_plus_1[j];

                        // If the filter appears in the combination, mark it
                        if (is_filter_in_combination(filter, combination))
                            mcs_filter_match[j] = true;
                    }

                    // If more than one filter is matched, mark all of them as unprotected
                    size_t matched_count = count(mcs_filter_match.begin(), mcs_filter_match.end(), true);
                    if (matched_count > 1) {
                        for (size_t j = 0; j < mcs_filter_match.size(); ++j) {
                            if (mcs_filter_match[j]) {
                                mcs_filter_protected[j] = false;
                            }
                        }
                    }

                    print_progress(++processed, total_reduced);
                }

                // == Phase 2.3: Randomly remove an unprotected filter from mcs[i]
                cout << "\n[PositionalMCS] Removing one unprotected filter...\n";

                vector<size_t> unprotected_indices;
                for (size_t j = 0; j < mcs_filter_protected.size(); ++j) {
                    if (!mcs_filter_protected[j]) {
                        unprotected_indices.push_back(j);
                    }
                }

                if (!unprotected_indices.empty()) {
                    // Choose a random index from the unprotected list
                    random_device rd;
                    mt19937 gen(rd());
                    uniform_int_distribution<> dis(0, static_cast<int>(unprotected_indices.size()) - 1);
                    size_t to_remove_index = unprotected_indices[dis(gen)];

                    // Remove the unprotected filter from mcs_i_plus_1
                    const string& removed_filter = mcs_i_plus_1[to_remove_index];
                    cout << "[PositionalMCS] Removing unprotected filter: " << removed_filter << endl;

                    auto it = find(mcs_i_plus_1.begin(), mcs_i_plus_1.end(), removed_filter);
                    if (it != mcs_i_plus_1.end()) {
                        mcs_i_plus_1.erase(it);
                    }
                }
                else {
                    // === Step 3: Save MCS[i+1] and return back to step 2 - keep on doing that until combinations are all deleted.
                    mcs_index++;
                    cout << "[PositionalMCS] All filters are protected. MCS[" << mcs_index << "] has been completed.\n";
                    all_mcs.push_back(mcs_i_plus_1);
                    mcs_i_plus_1.clear();
                    should_continue = false; // Once all are protected - no need to repeat 2.2 & 2.3
                }
            }
        }

        // === Step 4: Save each MCS[i] to separate files ===
        for (size_t i = 0; i < all_mcs.size(); ++i) {
            string filename = POSITIONAL_MCS_OUTPUT_FILENAME + "_" + to_string(i);
            int status = save_to_file(all_mcs[i], filename);
            if (status == 0)
                cout << "[PositionalMSC] MCS[" << i << "] saved with " << all_mcs[i].size() << " entries to " << filename << ".\n";
        }

        return 0; // return 0 if all files saved
	}
}