#define STB_IMAGE_IMPLEMENTATION
#include "level.hpp"
#include "stb_image.h" // Include stb_image header
#include <iostream>

bool Level::Load(const std::string& pngPath) {
    int x, y, n;
    unsigned char* data = stbi_load(pngPath.c_str(), &x, &y, &n, 3); // Request 3 channels (RGB)
    if (!data) {
        std::cerr << "ERROR: Could not load PNG file: " << pngPath << "\n";
        return false;
    }

    width = x;
    height = y;

    std::cout << "Width: " << width << ", Height: " << height << "\n";

    imageData.resize(height, std::vector<Pixel>(width));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char* pixel = &data[(y * width + x) * 3];
            // Swap R and B channels
            imageData[y][x].r = pixel[2]; // Red channel (was Blue)
            imageData[y][x].g = pixel[1]; // Green channel
            imageData[y][x].b = pixel[0]; // Blue channel (was Red)

            // Debug output for first few pixels
            /*if (y == 0 && x < 5) {
                std::cout << "Pixel at (" << x << "," << y << ") Original RGB: ("
                    << static_cast<int>(pixel[0]) << ","
                    << static_cast<int>(pixel[1]) << ","
                    << static_cast<int>(pixel[2]) << ") "
                    << "Converted RGB: ("
                    << static_cast<int>(imageData[y][x].r) << ","
                    << static_cast<int>(imageData[y][x].g) << ","
                    << static_cast<int>(imageData[y][x].b) << ")\n";
            }*/
        }
    }

    stbi_image_free(data); // Free the image data after usage
    return true;
}

void Level::Render() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& pixel = imageData[y][x];
            // Debug output for first few pixels during rendering
            /*if (y == 0 && x < 5) {
                std::cout << "Rendering pixel at (" << x << "," << y << ") RGB: ("
                    << static_cast<int>(pixel.r) << ","
                    << static_cast<int>(pixel.g) << ","
                    << static_cast<int>(pixel.b) << ")\n";
            }*/
            Renderer::DrawRect(x, y, 1, 1, pixel.r, pixel.g, pixel.b);
        }
    }
}