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

    // Default constructor
    TreeNode() : father(nullptr) {
        for (size_t i = 0; i < SIZE; ++i)
            pointers[i] = nullptr;
    }

    // Constructor with father
    TreeNode(TreeNode* parent) : father(parent) {
        for (size_t i = 0; i < SIZE; ++i)
            pointers[i] = nullptr;
    }

};

#endif // TREE_NODE_H
