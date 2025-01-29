#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "renderer.hpp"
#include "tilemap.hpp"
#include "levelData.hpp"

class Level {
public:
    // Function to load level data from specified JSON and tilemap paths
    bool Load(const std::string& jsonPath, const std::string& tilemapPath, int tilewidth, int tileheight);

    // Function to render the level using the provided device context
    void Render(HDC hdc);

private:
    Tilemap tilemap; // Object to manage the tilemap of the level
    LevelData levelData; // Object to hold the level's data
};

#endif