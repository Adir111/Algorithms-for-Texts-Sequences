#include "positional_mcs_search.hpp"

using namespace std;
using namespace Utils;
using namespace Config;
using namespace FiltersMap;

namespace PositionalMCSSearch {

	int run_positional_mcs_search() {
        cout << "[PositionalMCSSearch] Starting usual MCS search...\n";

        // Load Text
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[PositionalMCSSearch] Failed to load text - its empty or doesn`t exist.\n";
            return -1;
        }

        // Load MCS filters
        vector<vector<string>> all_mcs;
        int i = 0;
        while (i >= 0) {
            string filename = POSITIONAL_MCS_OUTPUT_FILENAME + "_" + to_string(i);
            vector<string> curr_mcs_filters = read_lines_from_file(filename);
            if (curr_mcs_filters.empty()) {
                if (i == 0) {
                    cerr << "[PositionalMCSSearch] Failed to load MCS filters - its empty or doens`t exist.\n";
                    return -1;
                }
                i = -1;
            }
            all_mcs.push_back(curr_mcs_filters);
            i++;
        }
        cout << "[PositionalMCSSearch] Successfully loaded all MCS files.\n";

        vector<string> mcs_filters = read_lines_from_file(STANDARD_MCS_OUTPUT_FILENAME);
        if (mcs_filters.empty()) {
            cerr << "[PositionalMCSSearch] Failed to load MCS filters - its empty or doens`t exist.\n";
            return -1;
        }

        // Load search words
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[PositionalMCSSearch] Failed to load search words.\n";
            return -1;
        }

        // Load filters map
        if (filters_map.empty()) {
            cerr << "[PositionalMCSSearch] Failed to load filters map - its empty.\n";
            return -1;
        }
	}
}