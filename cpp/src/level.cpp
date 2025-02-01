#define STB_IMAGE_IMPLEMENTATION
#include "level.hpp"
#include "stb_image.h" // Include stb_image header
#include <iostream>

bool Level::Load(const std::string& pngPath, int screenWidth, int screenHeight) {
    int x, y, n;
    unsigned char* data = stbi_load(pngPath.c_str(), &x, &y, &n, 3); // Request 3 channels (RGB)
    if (!data) {
        std::cerr << "ERROR: Could not load PNG file: " << pngPath << "\n";
        return false;
    }

    width = x;
    height = y;

    std::cout << "Width: " << width << ", Height: " << height << "\n";

    // Resize the image data to hold RGBA values
    imageData.resize(width * height * 4);

    // Convert BGR to RGB and store in imageData
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char* pixel = &data[(y * width + x) * 3];
            int index = (y * width + x) * 4;
            imageData[index + 0] = pixel[2]; // Red channel 
            imageData[index + 1] = pixel[1]; // Green channel
            imageData[index + 2] = pixel[0]; // Blue channel 
            imageData[index + 3] = 255;      // Alpha channel
        }
    }

    stbi_image_free(data); // Free the image data after usage
    return true;
}

void Level::Render(std::vector<unsigned char>& framebuffer, int screenWidth, int screenHeight) {
    // Copy the PNG image data into the framebuffer
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcIndex = (y * width + x) * 4; // Index in the PNG image data
            int dstIndex = ((y * screenWidth) + x) * 4; // Index in the framebuffer

            if (x >= screenWidth || y >= screenHeight) continue;

            framebuffer[dstIndex + 0] = imageData[srcIndex + 0]; // Red
            framebuffer[dstIndex + 1] = imageData[srcIndex + 1]; // Green
            framebuffer[dstIndex + 2] = imageData[srcIndex + 2]; // Blue
            framebuffer[dstIndex + 3] = imageData[srcIndex + 3]; // Alpha
        }
    }
}