#ifndef TREE_ARRAY_ELEMENT_H
#define TREE_ARRAY_ELEMENT_H

#include <cstddef>
#include "config.hpp"
#include "WordMatch.hpp"
#include <array>


struct TreeArrayElement {
    //int father;
    std::array<int, Config::SIZE> pointers{}; // Can point to next element or to word
    std::array<bool, Config::SIZE> is_leaf{};

    // Default constructor
    TreeArrayElement() {
        pointers.fill(-1);
        is_leaf.fill(false);
    }

    //// Custom constructor
    //TreeArrayElement(int parent) : father(parent) {
    //    pointers.fill(-1);
    //    is_leaf.fill(false);
    //}

};

#endif // TREE_ARRAY_ELEMENT_H
