#include "utils.hpp"

using namespace std;
using namespace chrono;
using namespace Config;

namespace Utils {

    bool has_generated_mcs = false;
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
     * @brief Determines whether a timer should be set for the given choice.
     *
     * @param choice The user choice.
     * @return true if a timer is needed, false otherwise.
     */
    static bool should_set_timer(int choice) {
        return find(TIMER_CHOICES.begin(), TIMER_CHOICES.end(), choice) != TIMER_CHOICES.end();
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
            switch (choice) {
            case 3:
                if (!has_generated_text) {
                    cerr << "Error: Cannot run \"" << OPTIONS[choice] << "\" before generating text (Option 2).\n";
                    return;
                }
                break;
            case 4:
                if (!has_generated_text) {
                    cerr << "Error: Cannot run \"" << OPTIONS[choice] << "\" before generating text (Option 2).\n";
                    return;
                }
                if (!has_generated_search_words) {
                    cerr << "Error: Cannot run \"" << OPTIONS[choice] << "\" before generating search words (Option 3).\n";
                    return;
                }
                break;
            case 5:
                if (!has_generated_mcs) {
                    cerr << "Error: Cannot run \"" << OPTIONS[choice] << "\" before creating MCS (Option 1).\n";
                    return;
                }
                if (!has_generated_text) {
                    cerr << "Error: Cannot run \"" << OPTIONS[choice] << "\" before generating text (Option 2).\n";
                    return;
                }
                break;
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
            has_generated_mcs = true;
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
     *               - 2: Random Text Generation
     *               - 3: Search Words Generation (requires step 2)
     *               - 4: Naive Search (requires steps 2 and 3)
     *               - 5: Filters Map Creation (requires steps 1 and 2)
     */
    void handle_operation(int (*operation)(), int choice) {
        // Dependency checks
        validate_choice(choice);

        // Timer start if choice is one of the searches
        auto start = (should_set_timer(choice)) ? steady_clock::now() : steady_clock::time_point();

        // Run operation
        if (operation() != 0) {
            cerr << "Error: " << OPTIONS[choice] << " failed.\n";
            return;
        }

        // Timer end and report duration if choice is 5
        if (should_set_timer(choice)) {
            auto end = steady_clock::now();
            duration<double> elapsed_seconds = end - start;
            cout << "[Utils] Operation \"" << OPTIONS[choice] << "\" completed in "
                << fixed << setprecision(2) << elapsed_seconds.count() << " seconds.\n";
        }


        // Set flags based on operation step
        update_flags(choice);
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
     * @return int 0 on success, -1 on failure
     */
    int save_to_file(const vector<string>& lines, const string& filename) {
        // Add .txt extension if needed
        string final_filename = ensure_txt_extension(filename);
        cout << "[Utils] Saving to file: " << final_filename << "\n";

        if (file_exists(final_filename)) {
            cout << "[Utils] File already exists. Overwrite? (y/n): ";
            char choice;
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
     * @return string The content read from the file, or empty on failure
     */
    string read_text_from_file(const string& filename) {
        // Ensure filename ends with .txt
        string final_filename = ensure_txt_extension(filename);

        // Open file for reading
        ifstream in(final_filename, ios::binary);
        if (!in) {
            cerr << "[Utils] Failed to open file: " << final_filename << '\n';
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
     * @return vector<string> List of lines (words).
     */
    vector<string> read_lines_from_file(const string& filename) {
        vector<string> lines;
        string content = read_text_from_file(filename);
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
        cout << "[Utils] Converting matches to output lines...\n";

        vector<string> lines;
        size_t total = matches.size();

        // Process each match object
        for (size_t i = 0; i < total; ++i) {
            const auto& match = matches[i];
            string line = "Word: " + match.word + "\nPositions: ";


            // Iterate over the set of positions
            size_t count = 0;
            for (auto pos : match.positions) {
                line += to_string(pos);
                if (++count < match.positions.size()) {
                    line += ", ";
                }
            }

            lines.push_back(line + "\n");

            print_progress(static_cast<int>(i), static_cast<int>(total));
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
        if (percent == 100.0) cout << endl;
    }

    ///** OUT OF USE - unable to read such large file. Saved in global variable instead.
    // * @brief Loads filters map from a file into an unordered_map.
    // *
    // * Each line is expected to start with a key (string) followed by integer positions.
    // *
    // * @param filename Path to the filters map file
    // * @return unordered_map<string, vector<int>> map of filtered words to positions
    // */
    //unordered_map<string, vector<int>> load_filters_map(const string& filename) {
    //    unordered_map<string, vector<int>> filters_map;

    //    string final_filename = filename;
    //    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
    //        final_filename += ".txt";
    //    }

    //    ifstream in(final_filename);
    //    if (!in) {
    //        cerr << "[Utils] Failed to open file: " << final_filename << '\n';
    //        return filters_map;
    //    }

    //    string line;
    //    int line_count = 0;
    //    // Read line by line
    //    while (getline(in, line)) {
    //        if (line.empty()) continue;

    //        istringstream iss(line);
    //        string key;
    //        iss >> key;
    //        vector<int> positions;
    //        int pos;
    //        while (iss >> pos) {
    //            positions.push_back(pos);
    //        }

    //        if (!key.empty()) {
    //            filters_map[key] = positions;
    //        }

    //        ++line_count;
    //        if (line_count % 10000 == 0) { // print progress every 10k lines
    //            cout << "[Utils] Processed lines: " << line_count << '\n';
    //        }
    //    }

    //    cout << "[Utils] Finished reading " << line_count << " lines from " << final_filename << "\n";
    //    return filters_map;
    //}

    /**
     * @brief Inserts a WordMatch into the set, or updates the existing match with a new position.
     *
     * @param result_set Reference to the set of WordMatch entries.
     * @param word The word being matched.
     * @param pos The position to add to the word's match list.
     */
    void insert_or_update_match(set<WordMatch>& result_set, const string& word, size_t pos) {
        WordMatch match{ word, { pos } };
        pair<set<WordMatch>::iterator, bool> insert_result = result_set.insert(match);

        if (!insert_result.second) {
            WordMatch updated = *insert_result.first;
            result_set.erase(insert_result.first);
            updated.positions.insert(pos);
            result_set.insert(updated);
        }
    }
}