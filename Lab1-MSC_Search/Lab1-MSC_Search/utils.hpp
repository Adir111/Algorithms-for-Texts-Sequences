#ifndef UTILS_HPP
#define UTILS_HPP

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <chrono>

#include "WordMatch.hpp"
#include "config.hpp"

namespace Utils {
    
    extern bool has_generated_msc;
    extern bool has_generated_text;
    extern bool has_generated_search_words;

    /**
     * @brief Handles an operation, enforcing dependencies and tracking progress flags.
     *
     * Validates if the operation can run based on prior completed steps,
     * executes the operation, logs errors if any, and updates internal flags.
     *
     * @param operation Function pointer to the operation to execute (must return int).
     * @param choice Integer representing the selected operation choice.
     *               Valid choices:
     *               - 1: MSC Creation
     *               - 2: Random Text Generation
     *               - 3: Search Words Generation (requires step 2)
     *               - 4: Naive Search (requires steps 2 and 3)
     *               - 5: Filters Map Creation (requires steps 1 and 2)
     */
    void handle_operation(int (*operation)(), int step);

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
    int save_to_file(const vector<std::string>& lines, const std::string& filename);
    
    /**
     * @brief Reads full text content from a file.
     *
     * @param filename The input filename
     * @return string The content read from the file, or empty on failure
     */
    std::string read_text_from_file(const std::string& filename);

    /**
     * @brief Reads a newline-separated list of strings from a file.
     *
     * @param filename Name of the file to read.
     * @return vector<string> List of lines (words).
     */
    vector<std::string> read_lines_from_file(const std::string& filename);

    /**
     * @brief Converts WordMatch results into printable lines for file saving.
     *
     * @param matches Vector of WordMatch objects
     * @return vector<string> Formatted output lines
     */
    vector<std::string> convert_matches_to_lines(const vector<WordMatch>& matches);

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