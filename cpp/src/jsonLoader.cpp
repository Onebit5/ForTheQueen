#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <iostream>
#include "jsonLoader.hpp"

// JSON parser
// Method to load level data from a specified JSON file
LevelData JSONLoader::LoadLevel(const std::string& filepath) {
	char path[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, path);
	std::cout << "Current working directory: " << path << std::endl;

	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cerr << "Failed to open level file " << filepath << std::endl;
		exit(1);
	} 

	LevelData levelData = {};
	levelData.tilewidth = 16;
	levelData.tileheight = 16;
	std::string line, key;

	while (std::getline(file, line)) {
		// Parse the line
		if (line.find("\"level\":") != std::string::npos) {
			std::vector<std::vector<int>> grid;
			while (std::getline(file, line) && line.find("]") == std::string::npos) {
				size_t start = line.find("[");
				size_t end = line.find("]");
				if (start != std::string::npos) {
					std::vector<int> row;
					std::stringstream ss(line.substr(start + 1, end - start - 1));
					int value;
					while (ss >> value) {
						row.push_back(value);
						if (ss.peek() == ',') ss.ignore();
					}
					grid.push_back(row);
					std::cout << "Loaded row with " << row.size() << " elements" << std::endl;
				}
			}
			levelData.level = grid;
			std::cout << "Total rows loaded: " << levelData.level.size() << std::endl;
		}
		else if (line.find("\"colliders\":") != std::string::npos) {
			std::vector<int> colliders;
			std::stringstream ss(line.substr(line.find("[") + 1));
			int value;
			while (ss >> value) {
				colliders.push_back(value);
				if (ss.peek() == ',') ss.ignore();
			}
			levelData.colliders = colliders;
		}
		else if (line.find("\"interactables\":") != std::string::npos) {
			std::vector<int> interactables;
			std::stringstream ss(line.substr(line.find("[") + 1));
			int value;
			while (ss >> value) {
				interactables.push_back(value);
				if (ss.peek() == ',') ss.ignore();
			}
			levelData.interactables = interactables;
		}
		else if (line.find("\"tilewidth\":") != std::string::npos) {
			levelData.tilewidth = std::stoi(line.substr(line.find(":") + 1));
		}
		else if (line.find("\"tileheight\":") != std::string::npos) {
			levelData.tileheight = std::stoi(line.substr(line.find(":") + 1));
		}
	}

	file.close();
	return levelData;
}