#include <iostream>
#include <fstream>
#include "jsonLoader.hpp"
#include "json.hpp"

// JSON parser
// Method to load level data from a specified JSON file
LevelData JSONLoader::LoadLevel(const std::string& filePath) {
    LevelData levelData;
    
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "ERROR: Unable to open file " << filePath << "\n";
        return levelData;
    }

    nlohmann::json jsonData;
    file >> jsonData;
    
    try {
        levelData.tilewidth = jsonData.at("tilewidth").get<int>();
        levelData.tileheight = jsonData.at("tileheight").get<int>();
        levelData.width = jsonData.at("width").get<int>();
        levelData.height = jsonData.at("height").get<int>();

        if (!jsonData.contains("layers") || !jsonData["layers"].is_array() || jsonData["layers"].empty()) {
            std::cerr << "Error: JSON missing 'data' array or incorrect format.\n";
            return levelData;
        }

        const auto& layer = jsonData["layers"][0];
        if (!layer.contains("data") || !layer["data"].is_array()) {
            std::cerr << "Error: JSON missing 'data' array or incorrect format.\n";
            return levelData;
        }

        const auto& dataArray = layer["data"];

        // Resize tileID to the expected size
        levelData.level.resize(levelData.height, std::vector<int>(levelData.width, 0));

        // Read "data" array
        int index = 0;
        for (const auto& tile : dataArray) {
            if (index < levelData.tileID.size()) {
                levelData.tileID[index++] = tile.get<int>();
            }
            else {
                std::cerr << "Warning: Extra tile data ignored (too many elements in JSON).\n";
                break;
            }
        }

        std::cout << "Loaded level successfully!\n";
        std::cout << "Map size: " << levelData.width << "x" << levelData.height << "\n";
        std::cout << "Tile count: " << levelData.tileID.size() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << "\n";
    }

    return levelData;
}