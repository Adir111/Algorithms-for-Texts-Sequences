#include "random_text.hpp"

using namespace std;
using namespace Config;
using namespace Utils;

namespace RandomText {

    /**
     * @brief Generates random text of size TEXT_SIZE using chars from Y_LETTER to 'z',
     * and saves it to RANDOM_GENERATED_TEXT_FILENAME.txt.
     *
     * @return int Returns 0 on success, -1 if saving the file failed.
     */
    int generate_text_and_save() {
        // === Random text generation setup ===
        cout << "[RandomText] Generating random text of size " << TEXT_SIZE << "...\n";

        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<> dist(Y_LETTER, 'z');

        string text;
        text.reserve(TEXT_SIZE);

        for (int i = 0; i < TEXT_SIZE; ++i) {
            text += static_cast<char>(dist(gen));
            print_progress(i, TEXT_SIZE);
        }

        // === Save to file ===
        int status = save_to_file({ text }, RANDOM_GENERATED_TEXT_FILENAME);
        if (status == 0) cout << "[RandomText] Text generation complete.\n";
        return status;
    }

}