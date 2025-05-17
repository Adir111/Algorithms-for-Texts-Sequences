#include "utils.hpp"

using namespace std;

namespace Utils {
    bool has_generated_text = false;
    bool has_generated_search_words = false;

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
    void handle_operation(int (*operation)(), const string& name, int step) {
        // Dependency checks
        if (step == 3 && !has_generated_text) {
            cerr << "Error: Cannot run \"" << name << "\" before generating text (Option 2).\n";
            return;
        }
        if (step == 4) {
            if (!has_generated_text) {
                cerr << "Error: Cannot run \"" << name << "\" before generating text (Option 2).\n";
                return;
            }
            if (!has_generated_search_words) {
                cerr << "Error: Cannot run \"" << name << "\" before generating search words (Option 3).\n";
                return;
            }
        }

        // Run operation
        if (operation() != 0) {
            cerr << "Error: " << name << " failed.\n";
            return;
        }

        // Set flags based on operation step
        if (step == 2) {
            has_generated_text = true;
        }
        else if (step == 3) {
            has_generated_search_words = true;
        }
    }

    /**
    * @brief Prints the options menu.
    */
    void print_menu() {
        cout << "\nWhat would you like to do? (0 for exit)\n";
        cout << "  1. Run MSC Creation\n";
        cout << "  2. Generate Random Text\n";
        cout << "  3. Generate Search Words\n";
        cout << "  4. Run Naive Search\n";
        cout << "  5. Option 5 (Coming soon)\n";
        cout << "  6. Option 6 (Coming soon)\n";
        cout << "Enter your choice: ";
    }

    /**
     * @brief Saves a list of lines to a text file.
     *
     * @param lines The lines to write
     * @param filename The target filename
     * @return int 0 on success, -1 on failure
     */
    int save_to_file(const vector<string>& lines, const string& filename) {
        // Add .txt extension if needed
        string final_filename = ensure_txt_extension(filename);
        cout << "[Utils] Saving to file: " << final_filename << "\n";

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
     * @return string The content read from the file, or empty on failure
     */
    string read_text_from_file(const string& filename) {
        // Ensure filename ends with .txt
        string final_filename = ensure_txt_extension(filename);

        // Open file for reading
        ifstream in(final_filename);
        if (!in) {
            cerr << "[Utils] Failed to open file: " << final_filename << '\n';
            return "";
        }

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
     * @return vector<string> List of lines (words).
     */
    vector<string> read_lines_from_file(const string& filename) {
        vector<string> lines;
        string content = read_text_from_file(filename);
        if (content.empty()) return lines;

        stringstream ss(content);
        string line;
        while (getline(ss, line)) {
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
        cout << "[Utils] Converting matches to output lines...\n";

        vector<string> lines;

        // Process each match object
        for (const auto& match : matches) {
            string line = "Word: " + match.word + "\nPositions: ";

            // Concatenate all positions into a line
            for (size_t i = 0; i < match.positions.size(); ++i) {
                line += to_string(match.positions[i]);
                if (i != match.positions.size() - 1) line += ", ";
            }
            lines.push_back(line + "\n");
        }

        cout << "[Utils] Conversion complete. Total words: " << matches.size() << '\n';
        return lines;
    }
}