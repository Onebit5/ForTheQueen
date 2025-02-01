#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>
#include <string>
#include "renderer.hpp"

class Level {
public:
    bool Load(const std::string& pngPath);
    void Render();

private:
    struct Pixel {
        unsigned char r, g, b;
    };

    std::vector<std::vector<Pixel>> imageData; // 2D vector to store the image pixel data
    int width;
    int height;
};

#endif