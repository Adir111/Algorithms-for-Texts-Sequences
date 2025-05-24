#ifndef SUMMARY_HPP
#define SUMMARY_HPP

#include <string>
#include <vector>
#include <set>

using std::string;
using std::vector;
using std::set;

/**
 * @brief Represents summary of a search.
 */
struct Summary {
    string search_type;
    int matches_found = 0;
    double time_took = 0;

    /**
     * @brief Converts the Summary object into a vector of strings representing each line.
     *
     * @return vector<string> A vector of strings, each representing a line of the Summary object.
     */
    vector<string> to_lines() const {
        vector<string> lines;

        lines.push_back("Search Type: " + search_type);
        lines.push_back("Matches Found: " + std::to_string(matches_found));
        lines.push_back("Time took: " + std::to_string(time_took) + " Seconds");

        return lines;
    }
};

#endif
