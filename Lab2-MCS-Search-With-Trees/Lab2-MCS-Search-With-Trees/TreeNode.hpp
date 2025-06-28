#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <cstddef>
#include "config.hpp"
#include "WordMatch.hpp"

struct TreeNode {
    TreeNode* father;
    void* pointers[Config::SIZE]; // Can point to WordMatch* or TreeNode*
    bool is_leaf[Config::SIZE];

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
