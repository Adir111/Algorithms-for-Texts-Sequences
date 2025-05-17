#ifndef UTILS_HPP
#define UTILS_HPP

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

#include "WordMatch.hpp"
#include "config.hpp"

namespace Utils {
    
    extern bool has_generated_msc;
    extern bool has_generated_text;
    extern bool has_generated_search_words;

    /**
     * @brief Handles an operation and prints an error if it fails or if dependencies are not met.
     *
     * Also updates internal flags to track completed steps for dependency management.
     *
     * @param operation Function pointer to the operation to execute (must return int).
     * @param name Description of the operation (used in logs and error messages).
     * @param step Step identifier for the operation (used to enforce required execution order).
     *             - 2: Random Text Generation
     *             - 3: Search Words Generation (requires step 2)
     *             - 4: Naive Search (requires steps 2 and 3)
     */
    void handle_operation(int (*operation)(), const std::string& name, int step);

	/**
	 * @brief Prints the options menu.
	 */
	void print_menu();

    /**
     * @brief Saves a list of lines to a text file.
     *
     * @param lines The lines to write
     * @param filename The target filename
     * @return int 0 on success, -1 on failure
     */
    int save_to_file(const vector<string>& lines, const string& filename);
    
    /**
     * @brief Reads full text content from a file.
     *
     * @param filename The input filename
     * @return string The content read from the file, or empty on failure
     */
    string read_text_from_file(const string& filename);

    /**
     * @brief Reads a newline-separated list of strings from a file.
     *
     * @param filename Name of the file to read.
     * @return vector<string> List of lines (words).
     */
    vector<string> read_lines_from_file(const string& filename);

    /**
     * @brief Converts WordMatch results into printable lines for file saving.
     *
     * @param matches Vector of WordMatch objects
     * @return vector<string> Formatted output lines
     */
    vector<string> convert_matches_to_lines(const vector<WordMatch>& matches);

    /**
     * @brief Prints progress as a percentage with two decimal digits, in-place on the same line.
     *        Progress updates are throttled to avoid excessive output.
     *
     * @param current Current iteration (0-based).
     * @param total Total number of iterations (must be > 0).
     */
    void print_progress(int current, int total);
    
    }

#endif