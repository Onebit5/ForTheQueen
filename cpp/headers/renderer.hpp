#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <windows.h>
#include <vector>
#include <string>

namespace Renderer {
    // Initializes the rendering system with specified width, height, and window title
    bool Init(int width, int height, const char* title);

    // Clears the current rendering context
    void Clear();

    // Draws a rectangle at the specified position with given dimensions and color
    void DrawRect(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b);

    // Restores a specified area of the screen using the provided pixel data
    void RestoreArea(int x, int y, int width, int height, const std::vector<unsigned char>& source, int screenWidth, int screenHeight);

    // Presents the rendered frame to the display
    void Present();

    // Shuts down the rendering system and releases resources
    void Shutdown();

    // Processes any pending messages or events
    bool ProcessMessages();

    // Returns a reference to the framebuffer for direct access
    std::vector<unsigned char>& GetFramebuffer();
}


#endif