#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <vector>
#include <string>
#include "renderer.hpp"

class Level {
public:
    // Function to load an image from a PNG file and set the screen dimensions
    bool Load(const std::string& pngPath, int screenWidth, int screenHeight);

    // Function to render the level onto the framebuffer
    void Render(std::vector<unsigned char>& framebuffer, int screenWidth, int screenHeight);

private:
    // Struct to represent a pixel with RGB values
    struct Pixel {
        unsigned char r, g, b; // Red, Green, and Blue color components
    };

    std::vector<unsigned char> imageData; // Single contiguous vector for RGBA pixel data
    int width; // Width of the image
    int height; // Height of the image

    // Inline function to get the pixel color at specified coordinates
    inline Pixel GetPixel(int x, int y) const {
        int index = (y * width + x) * 4; // Calculate the index for the pixel in the image data
        return { imageData[index], imageData[index + 1], imageData[index + 2] }; // Return the RGB values
    }
};

#endif