#include "final_summary.hpp"

using namespace std;
using namespace Utils;
using namespace Config;

namespace FinalSummary {

    /**
     * @brief Appends the summary information to the given lines vector.
     *
     * @param lines The vector to append the summary lines to.
     * @param summary The Summary struct containing data to be appended.
     */
    void append_summary_to_lines(vector<string>& lines, const Summary& summary) {
        vector<string> summary_lines = summary.to_lines();
        lines.insert(lines.end(), summary_lines.begin(), summary_lines.end());
        lines.push_back("\n"); // Add a blank line after each section
    }

    /**
     * @brief Parses a vector of lines and populates the Summary struct.
     *
     * @param lines The list of lines to parse.
     * @param summary The Summary struct to populate.
     * @return true if parsing was successful, false otherwise.
     */
    bool parse_summary_lines(const vector<string>& lines, Summary& summary) {
        for (const string& line : lines) {
            if (line.find("Search Type:") == 0) {
                summary.search_type = line.substr(12);  // Remove "Search Type: "
            }
            else if (line.find("Matches Found:") == 0) {
                summary.matches_found = stoi(line.substr(14));  // Remove "Matches Found: "
            }
            else if (line.find("Time took:") == 0) {
                summary.time_took = stod(line.substr(11));  // Remove "Time took: "
            }
        }
        return !summary.search_type.empty() && summary.matches_found >= 0 && summary.time_took >= 0.0;
    }

    /**
     * @brief Creates a final summary by reading data from multiple files and organizing them.
     *
     * This function reads the summary files for different search types, extracts the relevant data,
     * stores it in the Summary struct, and writes a formatted final summary to a text file.
     *
     * @return int Status code indicating success (0) or failure (-1).
     */
    int create_final_summary() {
        cout << "[FinalSummary] Starting final summary creation...\n";

        // === Phase 1: Read summaries and organize them into Summary structs ===
        Summary naive_summary, standard_mcs_summary, positional_mcs_summary;

        // Read Naive Search Summary File
        vector<string> naive_summary_lines = read_lines_from_file(NAIVE_SEARCH_SUMMARY_FILENAME);
        if (naive_summary_lines.empty()) {
            cerr << "[FinalSummary] Failed to load naive search summary.\n";
            return -1;
        }
        if (!parse_summary_lines(naive_summary_lines, naive_summary)) {
            cerr << "[FinalSummary] Failed to parse naive search summary.\n";
            return -1;
        }

        // Read Standard MCS Search Summary File
        vector<string> standard_mcs_summary_lines = read_lines_from_file(STANDARD_MCS_SEARCH_SUMMARY_FILENAME);
        if (standard_mcs_summary_lines.empty()) {
            cerr << "[FinalSummary] Failed to load standard mcs search summary.\n";
            return -1;
        }
        if (!parse_summary_lines(standard_mcs_summary_lines, standard_mcs_summary)) {
            cerr << "[FinalSummary] Failed to parse standard mcs search summary.\n";
            return -1;
        }

        // Read Positional MCS Search Summary File
        vector<string> positional_mcs_summary_lines = read_lines_from_file(POSITIONAL_MCS_SEARCH_SUMMARY_FILENAME);
        if (positional_mcs_summary_lines.empty()) {
            cerr << "[FinalSummary] Failed to load positional mcs search summary.\n";
            return -1;
        }
        if (!parse_summary_lines(positional_mcs_summary_lines, positional_mcs_summary)) {
            cerr << "[FinalSummary] Failed to parse positional mcs search summary.\n";
            return -1;
        }

        // === Phase 2: Prepare summary lines for writing ===
        vector<string> final_summary_lines;

        // Add Title
        final_summary_lines.push_back("=== Final Summary ===\n");

        // === Section 1: Naive Search Summary ===
        final_summary_lines.push_back("== Naive Search Summary ==\n");
        append_summary_to_lines(final_summary_lines, naive_summary);

        // === Section 2: Standard MCS Search Summary ===
        final_summary_lines.push_back("== Standard MCS Search Summary ==\n");
        append_summary_to_lines(final_summary_lines, standard_mcs_summary);

        // === Section 3: Positional MCS Search Summary ===
        final_summary_lines.push_back("== Positional MCS Search Summary ==\n");
        append_summary_to_lines(final_summary_lines, positional_mcs_summary);

        // === Section 4: Additional Information ===
        final_summary_lines.push_back("== Additional Information ==\n");

        // Validation
        if (naive_summary.time_took > 0 && standard_mcs_summary.time_took > 0 && positional_mcs_summary.time_took > 0) {
            // Calculate speed improvements (in percentages)
            double standard_vs_naive = ((naive_summary.time_took - standard_mcs_summary.time_took) / standard_mcs_summary.time_took) * 100;
            double positional_vs_naive = ((naive_summary.time_took - positional_mcs_summary.time_took) / positional_mcs_summary.time_took) * 100;
            double positional_vs_standard = ((standard_mcs_summary.time_took - positional_mcs_summary.time_took) / positional_mcs_summary.time_took) * 100;

            // Append results
            ostringstream oss;
            oss << fixed << setprecision(2);
            oss << "Standard MCS Search is " << standard_vs_naive << "% faster than Naive Search.\n";
            oss << "Positional MCS Search is " << positional_vs_naive << "% faster than Naive Search.\n";
            oss << "Positional MCS Search is " << positional_vs_standard << "% faster than Standard MCS Search.\n";
            final_summary_lines.push_back(oss.str());

        }
        else {
            final_summary_lines.push_back("[Error: Invalid time values, unable to compute speed improvements.]\n");
        }

        final_summary_lines.push_back("\n");

        // === Section 5: Configuration Settings ===
        final_summary_lines.push_back("== Configuration Settings ==\n");
        final_summary_lines.push_back("Text size                       : " + to_string(TEXT_SIZE));
        final_summary_lines.push_back("Number of Search Words          : " + to_string(NUMBER_OF_SEARCH_WORDS));
        final_summary_lines.push_back("Y-Letter                        : " + string(1, Y_LETTER));
        final_summary_lines.push_back("Search Word Size (pattern)      : " + to_string(SEARCH_WORD_SIZE));
        final_summary_lines.push_back("Minimal Matches (min hits)      : " + to_string(MINIMAL_MATCHES));
        final_summary_lines.push_back("Filter Amount Of Matches        : " + to_string(FILTER_AMOUNT_OF_MATCHES));
        final_summary_lines.push_back("\n");



        // === Phase 3: Save to file ===
        int result = save_to_file(final_summary_lines, COMBINED_SUMMARY_FILENAME, true);
        if (result == -1) {
            cerr << "[FinalSummary] Failed to save the final summary to file.\n";
            return -1;
        }

        cout << "[FinalSummary] Final summary successfully created and saved as " << COMBINED_SUMMARY_FILENAME << "\n";
        return 0;
    }
}
