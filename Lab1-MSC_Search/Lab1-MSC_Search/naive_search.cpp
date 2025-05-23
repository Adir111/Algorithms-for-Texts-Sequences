#include "naive_search.hpp"

using namespace std;
using namespace Config;
using namespace Utils;
using namespace chrono;

namespace NaiveSearch {

    /**
     * @brief Finds all starting positions of a word in the given text using naive search.
     *
     * @param text The full text to search in.
     * @param word The word to search for.
     * @return vector<int> List of 1-based starting positions.
     */
    static vector<size_t> find_word_positions(const string& text, const string& word) {
        vector<size_t> positions;
        size_t text_len = text.length();
        size_t word_len = word.length();

        // Loop through the text to find matches
        for (size_t i = 0; i <= text_len - word_len; ++i) {
            int matches = 0;

            // Check each character of the word
            for (size_t j = 0; j < word_len; ++j) {
                if (text[i + j] == word[j])
                    matches++;
            }

            // If minimum matches are found, store the position (1-based index)
            if (matches >= MINIMAL_MATCHES) {
                positions.push_back(i + 1);
            }
        }

        return positions;
    }

    /**
     * @brief Performs naive search for all given words and writes results to file.
     *
     * @return int 0 on success, -1 on failure
     */
    int perform_naive_search() {
        cout << "[NaiveSearch] Starting naive search...\n";
        int count_total_finds = 0;

        // Load the text to search from file
        string text = read_text_from_file(RANDOM_GENERATED_TEXT_FILENAME);
        if (text.empty()) {
            cerr << "[NaiveSearch] Failed to load input text.\n";
            return -1;
        }

        // Load search words from file
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[NaiveSearch] Failed to load search words.\n";
            return -1;
        }

        set<WordMatch> unique_matches;
        auto start = steady_clock::now();

        // Search each unique word only once
        size_t total_words = search_words.size();
        size_t processed = 0;

        for (const auto& word : search_words) {
            vector<size_t> positions = find_word_positions(text, word);
            for (size_t pos : positions) {
                if (insert_or_update_match(unique_matches, word, pos))
                    count_total_finds++;
            }
            print_progress(static_cast<int>(++processed), static_cast<int>(total_words));
        }

        auto end = steady_clock::now();
        duration<double> elapsed_seconds = end - start;
        double seconds = elapsed_seconds.count();
        Summary summary = { "Naive Search", count_total_finds, seconds };

        // Prepare output lines
        vector<WordMatch> results_vector(unique_matches.begin(), unique_matches.end());
        vector<string> lines = convert_matches_to_lines(results_vector);
        int status = save_to_file(lines, NAIVE_SEARCH_OUTPUT_FILENAME, true);
        if (save_to_file(summary.to_lines(), NAIVE_SEARCH_SUMMARY_FILENAME, true) != 0)
            cout << "[NaiveSearch] Summary failed saving.\n";
        else cout << "[NaiveSearch] Summary file has saved to " << NAIVE_SEARCH_SUMMARY_FILENAME << "\n";

        if (status == 0) cout << "[NaiveSearch] Search complete, found total of " << count_total_finds << ".\n";
        return status;
    }
}
