#ifndef JSON_LOADER_HPP
#define JSON_LOADER_HPP

#include <string>
#include <vector>
#include "levelData.hpp"

class JSONLoader {
public:
    static LevelData LoadLevel(const std::string& filePath); // Declare a static method to load level data from a JSON file
};

#endif
