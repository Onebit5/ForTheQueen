#include "tilemap.hpp"
#include <stdexcept>
#include <fstream>

// Function to load a tilemap from a specified file path
bool Tilemap::LoadTilemap(const std::string& filePath, int width, int height) {
    tilewidth = width; // Set the width of each tile
    tileheight = height; // Set the height of each tile

    // Load the bitmap from the specified file path
    std::ifstream file(filePath, std::ios::binary); // Open the file in binary mode
    if (!file.is_open()) { // Check if the file was successfully opened
        return false; // Return false if the file could not be opened
    }

    // Resize the tileImages vector to hold 16 tiles, each with specified width and height
    tileImages.resize(16, std::vector<unsigned char>(tilewidth * tileheight * 16, 255)); // Initialize with white color
    return true; // Return true indicating successful loading of the tilemap
}

// Function to render a specific tile at given coordinates
void Tilemap::RenderTile(int tileID, int x, int y, HDC hdc) {
    // Render a tile from `tileImages` at position (x, y)
    if (tileID < 0 || tileID >= tileImages.size()) return; // Check if tileID is valid

    // Placeholder logic to draw a filled rectangle for a tile
    Rectangle(hdc, x, y, x + tilewidth, y + tileheight); // Draw the rectangle representing the tile
}