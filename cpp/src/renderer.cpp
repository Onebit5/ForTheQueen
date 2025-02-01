#include "renderer.hpp"
#include <windows.h>
#include <vector>
#include <iostream>

// Internal variables
HWND hwnd; // Handle to the window
HDC hdc; // Handle to the device context
std::vector<unsigned char> framebuffer; // Framebuffer to store pixel data
int screenWidth, screenHeight; // Screen dimensions
bool running = true; // Flag to control the main loop

// Window Procedure for processing events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) // Switch statement to handle different window messages
    {
    case WM_DESTROY: // Handle window destruction
        PostQuitMessage(0); // Post a quit message to the message queue
        running = false; // Set running to false to exit the loop
        return 0; // Return 0 to indicate message has been processed
    case WM_CLOSE: // Handle window close event
        running = false; // Set running to false to exit the loop
        DestroyWindow(hwnd); // Destroy the window
        return 0; // Return 0 to indicate message has been processed
    default: // Handle any other messages
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Default processing
    }
}

namespace Renderer {
    bool Init(int width, int height, const char* title) {
        screenWidth = width; // Set the screen width
        screenHeight = height; // Set the screen height
        // Create framebuffer
        framebuffer.resize(screenWidth * screenHeight * 4, 255); // RGBA, initialized to white
        // Register Window Class
        WNDCLASS wc = {}; // Initialize window class structure
        wc.lpfnWndProc = WindowProc; // Set the window procedure
        wc.hInstance = GetModuleHandle(NULL); // Get the current instance handle
        wc.lpszClassName = "2D_Engine_Window"; // Set the window class name
        RegisterClass(&wc); // Register the window class
        // Create Window
        hwnd = CreateWindowEx(0, "2D_Engine_Window", title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
            NULL, NULL, GetModuleHandle(NULL), NULL); // Create the window
        if (!hwnd) return false; // Check if window creation failed
        ShowWindow(hwnd, SW_SHOW); // Show the window
        hdc = GetDC(hwnd); // Get the device context for the window
        return true; // Return true to indicate successful initialization
    }

    void Clear() {
        std::fill(framebuffer.begin(), framebuffer.end(), 255); // Fill framebuffer with white
    }

    void DrawRect(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b) {
        // Debug output for first few pixels during rendering
        /*if (y == 0 && x < 5) {
            std::cout << "Drawing rect at (" << x << ", " << y << "): "
                << "Size=" << width << "x" << height << ", "
                << "Color=(" << static_cast<int>(r) << ", "
                << static_cast<int>(g) << ", "
                << static_cast<int>(b) << ")\n";
        }*/

        for (int py = y; py < y + height; py++) { // Loop through each row
            for (int px = x; px < x + width; px++) { // Loop through each column
                if (px < 0 || px >= screenWidth || py < 0 || py >= screenHeight) // Check bounds
                    continue; // Skip if out of bounds
                int index = (py * screenWidth + px) * 4; // Calculate index for RGBA
                framebuffer[index + 0] = r; // Set red component
                framebuffer[index + 1] = g; // Set green component
                framebuffer[index + 2] = b; // Set blue component
                framebuffer[index + 3] = 255; // Set alpha component

                // Debug output for framebuffer updates
                /*if (py == 0 && px < 5) {
                    std::cout << "Framebuffer update at (" << px << "," << py << ") RGB: ("
                        << static_cast<int>(framebuffer[index + 0]) << ","
                        << static_cast<int>(framebuffer[index + 1]) << ","
                        << static_cast<int>(framebuffer[index + 2]) << ")\n";
                }*/
            }
        }
    }

    void Present() {
        BITMAPINFO bmi = {}; // Initialize bitmap info structure
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); // Set size of the header
        bmi.bmiHeader.biWidth = screenWidth; // Set width of the bitmap
        bmi.bmiHeader.biHeight = -screenHeight; // Set height (negative to flip vertically)
        bmi.bmiHeader.biPlanes = 1; // Set number of color planes
        bmi.bmiHeader.biBitCount = 32; // Set bits per pixel
        bmi.bmiHeader.biCompression = BI_RGB; // Set compression type
        bmi.bmiHeader.biSizeImage = screenWidth * screenHeight * 4; // Set image size

        // Draw the framebuffer to the window
        StretchDIBits(hdc, 0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight,
            framebuffer.data(), &bmi, DIB_RGB_COLORS, SRCCOPY); // Copy framebuffer to device context
    }

    void Shutdown() {
        ReleaseDC(hwnd, hdc); // Release the device context
        DestroyWindow(hwnd); // Destroy the window
    }

    bool ProcessMessages() {
        MSG msg; // Message structure
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Process messages in the queue
            if (msg.message == WM_QUIT) { // Check for quit message
                running = false; // Set running to false to exit the loop
            }
            TranslateMessage(&msg); // Translate the message
            DispatchMessage(&msg); // Dispatch the message to the window procedure
        }
        return running; // Return the running state
    }
}