#include "utils.hpp"

using namespace std;
using namespace chrono;
using namespace Config;

namespace Utils {

    bool has_generated_msc = false;
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

    static bool should_set_time(int choice) {
        if (choice == 5) return true;
        return false;
    }

    /**
     * @brief Validates whether the selected choice can be executed
     *        based on the current progress flags and dependencies.
     *
     * If dependencies are not met, error messages are printed and execution is halted.
     *
     * @param choice Integer representing the operation choice to validate.
     */
    static void validate_choice(int choice) {
        if (VALIDATE_SELECTIONS) {
            if (choice == 3 && !has_generated_text) {
                cerr << "Error: Cannot run \"" << choices[choice] << "\" before generating text (Option 2).\n";
                return;
            }
            if (choice == 4) {
                if (!has_generated_text) {
                    cerr << "Error: Cannot run \"" << choices[choice] << "\" before generating text (Option 2).\n";
                    return;
                }
                if (!has_generated_search_words) {
                    cerr << "Error: Cannot run \"" << choices[choice] << "\" before generating search words (Option 3).\n";
                    return;
                }
            }
            if (choice == 5) {
                if (!has_generated_msc) {
                    cerr << "Error: Cannot run \"" << choices[choice] << "\" before creating MSC (Option 1).\n";
                    return;
                }
                if (!has_generated_text) {
                    cerr << "Error: Cannot run \"" << choices[choice] << "\" before generating text (Option 2).\n";
                    return;
                }
            }
        }
    }

    /**
     * @brief Updates internal state flags based on the completed operation choice.
     *
     * Tracks which key steps have been successfully executed for dependency management.
     *
     * @param choice Integer representing the operation choice that just completed.
     */
    static void update_flags(int choice) {
        switch (choice) {
        case 1:
            has_generated_msc = true;
            break;
        case 2:
            has_generated_text = true;
            break;
        case 3:
            has_generated_search_words = true;
            break;
        }
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
     *               - 1: MSC Creation
     *               - 2: Random Text Generation
     *               - 3: Search Words Generation (requires step 2)
     *               - 4: Naive Search (requires steps 2 and 3)
     *               - 5: Filters Map Creation (requires steps 1 and 2)
     */
    void handle_operation(int (*operation)(), int choice) {
        // Dependency checks
        validate_choice(choice);

        // Timer start if choice is one of the searches
        bool should_set_timer = should_set_time(choice);
        auto start = (should_set_timer) ? steady_clock::now() : steady_clock::time_point();

        // Run operation
        if (operation() != 0) {
            cerr << "Error: " << choices[choice] << " failed.\n";
            return;
        }

        // Timer end and report duration if choice is 5
        if (should_set_timer) {
            auto end = steady_clock::now();
            duration<double> elapsed_seconds = end - start;
            cout << "[Utils] Operation \"" << choices[choice] << "\" completed in "
                << fixed << setprecision(2) << elapsed_seconds.count() << " seconds.\n";
        }


        // Set flags based on operation step
        update_flags(choice);
    }

    /**
     * @brief Prints the options menu.
     */
    void print_menu() {
        cout << "\nWhat would you like to do? (0 for exit)\n";
        cout << "  1. Run MSC Creation\n";
        cout << "  2. Generate Random Text\n";
        cout << "  3. Generate Search Words\n";
        cout << "  4. Create Filters Map\n";
        cout << "  5. Run Naive Search\n";
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
        if (current == total - 1) cout << endl;
    }
}