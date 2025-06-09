#include "utils.hpp"

using namespace std;
using namespace chrono;
using namespace Config;

namespace Utils {

    /**
     * @brief Ensures the filename has a .txt extension.
     *
     * @param filename The original filename
     * @return string Modified filename with .txt if missing
     */
    static string ensure_txt_extension(const string& filename) {
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
            return filename + ".txt";
        }
        return filename;
    }

    /**
     * @brief Checks whether a file already exists on disk.
     *
     * @param filename The name of the file to check (with or without path).
     * @return true if the file exists, false otherwise.
     */
    static bool file_exists(const std::string& filename) {
        std::ifstream infile(filename);
        return infile.good();
    }

    /**
     * @brief Handles an operation, enforcing dependencies and tracking progress flags.
     *
     * Validates if the operation can run based on prior completed steps,
     * executes the operation, logs errors if any, and updates internal flags.
     *
     * @param operation Function pointer to the operation to execute (must return int).
     * @param choice Integer representing the selected operation choice.
     *               Valid choices:
     *               - 1: MCS Creation
     *               - 2: Positional MCS Creation
     *               - 3: Random Text Creation
     *               - 4: Search Words Creation
     *               - 5: Filters Map Creation
     *               - 6: Naive Search
     *               - 7: Standard MCS Search
     *               - 8: Positional MCS Search
     *
     */
    void handle_operation(int (*operation)(), int choice) {
        if (operation() != 0) {
            cerr << "Error: " << OPTIONS[static_cast<size_t>(choice) - 1] << " failed.\n";
            return;
        }
    }

    /**
     * @brief Prints the options menu.
     */
    void print_menu() {
        cout << "\n========================================\n";
        cout << "\nWhat would you like to do? (0 for exit)\n";
        for (size_t i = 0; i < OPTIONS.size(); ++i) {
            cout << "  " << (i + 1) << ". Run " << OPTIONS[i] << "\n";
        }
        cout << "Enter your choice: ";
    }

    /**
     * @brief Saves a list of lines to a text file.
     *
     * @param lines The lines to write
     * @param filename The target filename
     * @param overwrite Should overwrite a file that exists or not, default to false.
     * @return int 0 on success, -1 on failure
     */
    int save_to_file(const vector<string>& lines, const string& filename, bool overwrite) {
        // Add .txt extension if needed
        string final_filename = ensure_txt_extension(filename);
        cout << "[Utils] Saving to file: " << final_filename << "\n";

        if (!overwrite && file_exists(final_filename)) {
            cout << "[Utils] File already exists. Overwrite? (y/n): ";
            char choice = 'n';
            cin >> choice;
            if (choice != 'y' && choice != 'Y') {
                cout << "[Utils] Save cancelled by user.\n";
                return -2;
            }
        }

        // Open file for writing
        ofstream out(final_filename);
        if (!out) {
            cerr << "[Utils] Failed to open file: " << final_filename << '\n';
            return -1;
        }

        // Write each line to file
        for (const auto& line : lines) {
            out << line << '\n';
        }

        cout << "[Utils] File saved successfully: " << final_filename << '\n';
        return 0;
    }

    /**
     * @brief Reads full text content from a file.
     *
     * @param filename The input filename
     * @param should_print_log Flag indicating if should print logs.
     * @return string The content read from the file, or empty on failure
     */
    string read_text_from_file(const string& filename, bool should_print_log) {
        // Ensure filename ends with .txt
        string final_filename = ensure_txt_extension(filename);

        // Open file for reading
        ifstream in(final_filename, ios::binary);
        if (!in) {
            if (should_print_log) cerr << "[Utils] Failed to open file: " << final_filename << '\n';
            return "";
        }

        in.clear(); // reset any flags
        in.seekg(0, ios::beg); // ensure we're at the start

        // Read full content into buffer
        stringstream buffer;
        buffer << in.rdbuf();

        cout << "[Utils] Loaded text from file: " << final_filename << '\n';
        return buffer.str();
    }

    /**
     * @brief Reads a newline-separated list of strings from a file.
     *
     * @param filename Name of the file to read.
     * @param should_print_log Flag indicating if should print logs. Default to true.
     * @return vector<string> List of lines (words).
     */
    vector<string> read_lines_from_file(const string& filename, bool should_print_log) {
        vector<string> lines;
        string content = read_text_from_file(filename, should_print_log);
        if (content.empty()) return lines;

        stringstream ss(content);
        string line;
        while (getline(ss, line)) {
            // Remove trailing carriage return if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        return lines;
    }

    /**
     * @brief Converts WordMatch results into printable lines for file saving.
     *
     * @param matches Vector of WordMatch objects
     * @return vector<string> Formatted output lines
     */
    vector<string> convert_matches_to_lines(const vector<WordMatch>& matches) {
        std::cout << "[Utils] Converting matches to output lines...\n";

        vector<string> lines;
        size_t total = matches.size();

        // Process each match object
        for (size_t i = 0; i < total; ++i) {
            const auto& match = matches[i];
            lines.push_back(match.to_string() + "\n");

            print_progress(static_cast<int>(i), static_cast<int>(total));
        }

        std::cout << "[Utils] Conversion complete. Total words: " << matches.size() << '\n';
        return lines;
    }

    /**
     * @brief Prints progress as a percentage with two decimal digits, in-place on the same line.
     *        Progress updates are throttled to avoid excessive output.
     *
     * @param current Current iteration (0-based).
     * @param total Total number of iterations (must be > 0).
     */
    void print_progress(int current, int total) {
        if (total <= 0) return;

        const int update_interval = max(1, total / PROGRESS_UPDATES_COUNT);
        if (current % update_interval != 0 && current != total - 1) return;

        double percent = (static_cast<double>(current + 1) / total) * 100.0;
        if (percent > 100.0) percent = 100.0;

        ostringstream out;
        out << "\r[Progress] " << fixed << setprecision(2) << percent << "%";
        cout << out.str() << flush;

        // Print newline only when reaching 100%
        if (percent >= 100.0) cout << endl;
    }

    /**
     * @brief Inserts a WordMatch into the set, or updates the existing match with a new position.
     *
     * @param result_set Reference to the set of WordMatch entries.
     * @param word The word being matched.
     * @param pos The position to add to the word's match list.
     * @return true if the position was added successfully, false if it already existed.
     */
    bool insert_or_update_match(set<WordMatch>& result_set, const string& word, size_t pos) {
        WordMatch match{ word, { pos } };
        auto insert_result = result_set.insert(match);

        if (insert_result.second) {
            // Successfully inserted new match with pos
            return true;
        }
        else {
            WordMatch updated = *insert_result.first;
            if (updated.positions.count(pos)) {
                // Position already exists
                return false;
            }
            result_set.erase(insert_result.first);
            updated.positions.insert(pos);
            result_set.insert(updated);
            return true;
        }
    }
}