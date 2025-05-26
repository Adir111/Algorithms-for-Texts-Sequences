#ifndef FINAL_SUMMARY_HPP
#define FINAL_SUMMARY_HPP

#include <string>
#include "utils.hpp"
#include "config.hpp"
#include "Summary.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

namespace FinalSummary {

    /**
	 * @brief Creates a final summary by reading data from multiple files and organizing them.
	 *
	 * This function reads the summary files for different search types, extracts the relevant data,
	 * stores it in the Summary struct, and writes a formatted final summary to a text file.
	 *
	 * @return int Status code indicating success (0) or failure (-1).
	 */
	int create_final_summary();
}

#endif
