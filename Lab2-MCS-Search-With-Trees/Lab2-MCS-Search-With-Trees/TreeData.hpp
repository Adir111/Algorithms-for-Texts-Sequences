#ifndef TREE_DATA_HPP
#define TREE_DATA_HPP

#include "TreeNode.hpp"
#include <unordered_map>
#include <set>
#include <string>


struct TreeData {
    TreeNode* tree = nullptr;
    std::unordered_map<std::string, std::set<size_t>> filters_map;
};

extern TreeData tree_data;


#endif
