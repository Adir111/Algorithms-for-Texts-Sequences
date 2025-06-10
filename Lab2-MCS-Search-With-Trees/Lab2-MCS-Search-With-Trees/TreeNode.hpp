#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <cstddef>
#include "config.hpp"
#include "WordMatch.hpp"

// Set your desired SIZE
constexpr size_t SIZE = 'z' - Config::Y_LETTER + 2; // an extra position for $

struct TreeNode {
    TreeNode* father;
    void* pointers[SIZE]; // Can point to WordMatch* or TreeNode*
    bool is_leaf[SIZE];

    // Default constructor
    TreeNode() : father(nullptr) {
        std::fill(std::begin(pointers), std::end(pointers), nullptr);
        std::fill(std::begin(is_leaf), std::end(is_leaf), false);
    }

    // Constructor with father
    TreeNode(TreeNode* parent) : father(parent) {
        std::fill(std::begin(pointers), std::end(pointers), nullptr);
        std::fill(std::begin(is_leaf), std::end(is_leaf), false);
    }

};

#endif // TREE_NODE_H
