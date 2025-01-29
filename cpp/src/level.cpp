#include "level.hpp"
#include "jsonLoader.hpp"

// Function to load level data from a JSON file and tilemap
bool Level::Load(const std::string& jsonPath, const std::string& tilemapPath, int tileWidth, int tileHeight) {
    levelData = JSONLoader::LoadLevel(jsonPath); // Load level data from the specified JSON path
    return tilemap.LoadTilemap(tilemapPath, tileWidth, tileHeight); // Load the tilemap and return success status
}

// Function to render the level on the given device context
void Level::Render(HDC hdc) {
    // Iterate through each row of the level grid
    for (int y = 0; y < levelData.levelGrid.size(); ++y) {
        // Iterate through each column of the level grid
        for (int x = 0; x < levelData.levelGrid[y].size(); ++x) {
            int tileID = levelData.levelGrid[y][x]; // Get the tile ID from the level grid
            // Render the tile at the calculated position using the device context
            tilemap.RenderTile(tileID, x * levelData.tilewidth, y * levelData.tilewidth, hdc);
        }
    }
}