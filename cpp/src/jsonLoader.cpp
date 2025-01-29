#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <Windows.h>
#include <iostream>
#include "jsonLoader.hpp"

// JSON parser
// Method to load level data from a specified JSON file
LevelData LoadLevel(const std::string& filepath) {
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cerr << "Failed to open level file " << filepath << std::endl;
		exit(1);
	}

	LevelData levelData = {};
	levelData.tilewidth = 16;
	std::string line, key;

	while (std::getline(file, line)) {
		// Parse the line
		if (line.find("\"level\":") != std::string::npos) {
			std::vector<std::vector<int>> grid;
			while (std::getline(file, line) && line.find("]") == std::string::npos) {
				if (line.find("[") != std::string::npos) {
					std::vector<int> row;
					std::stringstream ss(line.substr(line.find("[") + 1));
					int value;
					while (ss >> value) {
						row.push_back(value);
						if (ss.peek() == ',') ss.ignore();
					}
					grid.push_back(row);
				}
			}
			levelData.levelGrid = grid;
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
	}

	file.close();
	return levelData;
}