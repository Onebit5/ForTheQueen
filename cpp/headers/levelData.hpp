#ifndef LEVELDATA_HPP
#define LEVELDATA_HPP

#include <vector>

struct LevelData {
	std::vector<std::vector<int>> levelGrid; // Tile grid
	std::vector<int> colliders; // Tiles that are colliders
	std::vector<int> interactables; // Tiles that are interactable
	int tilewidth = 16; // Default tile size
};

#endif
