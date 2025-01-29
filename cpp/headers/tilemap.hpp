#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <vector>
#include <windows.h>

class Tilemap {
public:
    // Function to load a tilemap from a specified file path with given tile dimensions
    bool LoadTilemap(const std::string& filePath, int tilewidth, int tileheight);

    // Function to render a specific tile at given coordinates using a device context
    void RenderTile(int tileID, int x, int y, HDC hdc);

private:
    // 2D vector to store tile images as unsigned char values
    std::vector<std::vector<unsigned char>> tileImages;
    int tilewidth; 
    int tileheight;
};

#endif