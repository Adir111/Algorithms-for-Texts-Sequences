#ifndef TREE_DATA_ARRAY_HPP
#define TREE_DATA_ARRAY_HPP

#include "TreeArrayElement.hpp"
#include <unordered_map>
#include <set>
#include <string>
#include "WordMatch.hpp"


struct TreeDataArray {
    std::vector<TreeArrayElement> tree;
    std::vector<WordMatch> filters_positions;
    //std::unordered_map<std::string, std::set<size_t>> filters_map;
};

extern TreeDataArray tree_data_array;


#endif
