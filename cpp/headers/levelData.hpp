#ifndef LEVELDATA_HPP
#define LEVELDATA_HPP

#include <vector>

struct LevelData {
	std::vector<std::vector<int>> level; // Tile grid
	std::vector<int> colliders; // Tiles that are colliders
	std::vector<int> interactables; // Tiles that are interactable
	int tilewidth = 16; // Default tile width
	int tileheight = 16; // Default tile height
	int width = 80; // Default map width
	int height = 45; // Default map height
	std::vector<int> tileID; // Tile IDs
};

#endif
