#include "create_text_with_search_words.hpp"

using namespace std;
using namespace Config;
using namespace Utils;

namespace SearchWordsAndTextGenerator {

    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> char_dist(Y_LETTER, 'z');

    /**
     * @brief Generates a list of search words from the random text file
     *        and saves it to a file.
     *
     * This function:
     * - Generates random search words of length SEARCH_WORD_SIZE.
     * - Each search word is built using chars from Y_LETTER to z.
     * - Generates NUMBER_OF_SEARCH_WORDS such words.
     * - Saves them to the file defined by SEARCH_WORDS_FILENAME.
     *
     * @return int 0 on success, -1 on failure
     */
    static int generate_search_words() {
        cout << "[TextAndSearchWordsGenerator] Generating " << NUMBER_OF_SEARCH_WORDS << " Search Words...\n";
        // Init
        vector<string> search_words;

        // Create all search words
        while (search_words.size() < NUMBER_OF_SEARCH_WORDS) {
            string curr_search_word = "";
            for (int i = 0; i < SEARCH_WORD_SIZE; i++)
                curr_search_word += static_cast<char>(char_dist(gen));
            search_words.push_back(curr_search_word);
            print_progress(static_cast<int>(search_words.size()), NUMBER_OF_SEARCH_WORDS);
        }

        // Save the vector to a file
        int status = save_to_file(search_words, SEARCH_WORDS_FILENAME);
        if (status == 0) cout << "[TextAndSearchWordsGenerator] Successfully generated and saved " << search_words.size() << " search words.\n";
        return status;
    }

    /**
     * @brief Generates random word variants by modifying random characters.
     *
     * This function creates `NUMBER_OF_VARIATIONS` copies of the input `word`, with
     * each variant having a random number of character changes (between 1 and SEARCH_WORD_SIZE - MINIMAL_MATCHES,
     * based on the word size). The changes are applied at random positions within
     * the word.
     *
     * @param word The original word to generate variants from.
     *
     * @return A vector of word variants with random character changes.
     */
    static vector<string> get_word_variants(const string& word) {
        // Max number of changes possible for the word to fit search condition
        int max_number_of_changes = SEARCH_WORD_SIZE - MINIMAL_MATCHES;
        uniform_int_distribution<> num_dist_changes(1, max_number_of_changes);
        uniform_int_distribution<> num_dist_index(0, SEARCH_WORD_SIZE - 1);
        int number_of_iterations = num_dist_changes(gen);
        vector<string> variants;

        for (int curr_variant = 0; curr_variant < NUMBER_OF_VARIATIONS; curr_variant++) {
            string word_variant = word;
            for (int iter = 0; iter < number_of_iterations; iter++) {
                // It might change a letter it changed before, do not mind that.
                int random_index = num_dist_index(gen);
                char random_char = static_cast<char>(char_dist(gen));
                word_variant[random_index] = random_char;
            }
            variants.push_back(word_variant);
        }
        return variants;
    }

    /**
     * @brief Generates random text by inserting search words and their variants at random positions.
     *
     * This function generates a random text of size `TEXT_SIZE` by inserting a list of search words
     * and their variants. Each word and its variants are inserted at random positions in the text,
     * and no positions are overwritten. After all words have been inserted, the remaining empty
     * positions are filled with random characters.
     *
     * @return int 0 on success, non-zero value on failure.
     */
    static int generate_text() {
        cout << "[TextAndSearchWordsGenerator] Generating random text of size " << TEXT_SIZE << "...\n";

        // === Load search words ===
        vector<string> search_words = read_lines_from_file(SEARCH_WORDS_FILENAME);
        if (search_words.empty()) {
            cerr << "[MCSTreeSearch] Failed to load search words.\n";
            return -1;
        }

        // Init
        string text(TEXT_SIZE, ' ');
        unordered_set<size_t> taken_positions; // Used to track taken positions
        uniform_int_distribution<> num_dist_index(0, TEXT_SIZE - SEARCH_WORD_SIZE);
        int total_inserted = 0;
        
        cout << "[TextAndSearchWordsGenerator] Starting building text with search words and its variants..\n";
        for (const string& word : search_words) {
            vector<string> variants = get_word_variants(word);
            variants.push_back(word); // Add original word as well

            for (const string& variant : variants) {
                bool inserted = false;

                while (!inserted) {
                    // Generate a random starting index for the word/variant
                    size_t random_index = num_dist_index(gen);

                    // Check if the first and last position are available
                    if (taken_positions.find(random_index) == taken_positions.end() &&
                        taken_positions.find(random_index + SEARCH_WORD_SIZE - 1) == taken_positions.end()) {

                        // Insert the word/variant into the text at the random position
                        for (size_t i = 0; i < SEARCH_WORD_SIZE; i++) {
                            text[random_index + i] = variant[i];
                            taken_positions.insert(random_index + i);  // Mark position as taken
                        }
                        inserted = true;  // Successfully inserted the variant
                    }
                }

                total_inserted++;
                print_progress(total_inserted, NUMBER_OF_SEARCH_WORDS * (NUMBER_OF_VARIATIONS + 1));
            }
        }

        // Fill the remaining empty spaces with random characters
        cout << "[TextAndSearchWordsGenerator] Filling missing spaces with random letters..\n";
        for (int i = 0; i < TEXT_SIZE; i++) {
            if (taken_positions.find(i) == taken_positions.end()) {
                text[i] = static_cast<char>(char_dist(gen));
            }
        }

        // Save to file
        int status = save_to_file({ text }, RANDOM_GENERATED_TEXT_FILENAME);
        if (status == 0) cout << "[TextAndSearchWordsGenerator] Successfully generated random text.\n";
        return status;
    }

    /**
     * @brief Generates search words and random text, then saves them to files.
     *
     * This function performs two tasks:
     * 1. It generates a list of random search words and saves them to a file.
     * 2. It generates random text of size `TEXT_SIZE` and inserts the search words and their variants
     *    at random positions in the text. After inserting all words and variants, the remaining spaces
     *    are filled with random characters. Finally, the text is saved to a file.
     *
     * This function ensures that the total size of the search words (including variants) does not exceed
     * the size of the text. If the size condition is violated, it prints an error and terminates the program.
     *
     * @return int 0 on success, -1 on failure.
     */
    int generate_text_and_search_words() {
        if (NUMBER_OF_SEARCH_WORDS * SEARCH_WORD_SIZE * (NUMBER_OF_VARIATIONS + 1) > TEXT_SIZE) {
            cout << "[TextAndSearchWordsGenerator] CRITICAL ERROR - search words total size is bigger then text size! Can't generate the text with those values.\n";
            exit(-1);
        }

        if (generate_search_words() != 0) {
            cout << "[TextAndSearchWordsGenerator] Error with search words generation.\n";
            return -1;
        }

        return generate_text();
    }
}
