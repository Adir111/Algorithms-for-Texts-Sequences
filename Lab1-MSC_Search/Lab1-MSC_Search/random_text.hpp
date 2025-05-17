#ifndef RANDOM_TEXT_HPP
#define RANDOM_TEXT_HPP

#include <string>
#include <random>
#include <vector>

#include "config.hpp"
#include "utils.hpp"

namespace RandomText {
    /**
     * @brief Generates random text of size TEXT_SIZE using chars from Y_LETTER to 'z',
     * and saves it to RANDOM_GENERATED_TEXT_FILENAME.txt.
     *
     * @return int Returns 0 on success, -1 if saving the file failed.
     */
    int generate_text_and_save();
}

#endif