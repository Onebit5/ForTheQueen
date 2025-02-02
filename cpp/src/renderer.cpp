#include "renderer.hpp"
#include <windows.h>
#include <vector>
#include <cstring>
#include <iostream>

// Internal variables
HWND hwnd; // Handle to the window
HDC hdc; // Handle to the device context
std::vector<unsigned char> framebuffer; // Framebuffer to store pixel data
int screenWidth, screenHeight; // Screen dimensions
bool running = true; // Flag to control the main loop

// Window Procedure for processing events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0); // Post a quit message when the window is destroyed
        running = false; // Set running to false to exit the main loop
        return 0;
    case WM_CLOSE:
        running = false; // Set running to false to exit the main loop
        DestroyWindow(hwnd); // Destroy the window
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Default window procedure
    }
}

namespace Renderer {
    bool Init(int width, int height, const char* title) {
        screenWidth = width; // Set screen width
        screenHeight = height; // Set screen height

        // Create framebuffer
        framebuffer.resize(screenWidth * screenHeight * 4, 255); // RGBA, initialized to white

        // Register Window Class
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc; // Set the window procedure
        wc.hInstance = GetModuleHandle(NULL); // Get the instance handle
        wc.lpszClassName = "2D_Engine_Window"; // Set the window class name
        RegisterClass(&wc); // Register the window class

        // Create Window
        hwnd = CreateWindowEx(
            0, "2D_Engine_Window", title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
            NULL, NULL, GetModuleHandle(NULL), NULL
        );

        if (!hwnd) return false; // Return false if window creation fails

        ShowWindow(hwnd, SW_SHOW); // Show the window
        hdc = GetDC(hwnd); // Get the device context
        return true; // Return true if initialization is successful
    }

    void Clear() {
        std::fill(framebuffer.begin(), framebuffer.end(), 255); // Fill framebuffer with white
    }

    void DrawRect(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b) {
        for (int py = y; py < y + height; py++) {
            if (py < 0 || py >= screenHeight) continue;

            for (int px = x; px < x + width; px++) {
                if (px < 0 || px >= screenWidth) continue;

                int index = (py * screenWidth + px) * 4; // Calculate index for RGBA
                framebuffer[index + 0] = r; // Set red component
                framebuffer[index + 1] = g; // Set green component
                framebuffer[index + 2] = b; // Set blue component
                framebuffer[index + 3] = 255; // Set alpha component
            }
        }
    }

    void Present() {
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); // Set the size of the bitmap info header
        bmi.bmiHeader.biWidth = screenWidth; // Set the width of the bitmap
        bmi.bmiHeader.biHeight = -screenHeight; // Negative height flips vertically
        bmi.bmiHeader.biPlanes = 1; // Set the number of color planes
        bmi.bmiHeader.biBitCount = 32; // Set the number of bits per pixel
        bmi.bmiHeader.biCompression = BI_RGB; // Set the compression type

        StretchDIBits(
            hdc, 0, 0, screenWidth, screenHeight,
            0, 0, screenWidth, screenHeight,
            framebuffer.data(), &bmi, DIB_RGB_COLORS, SRCCOPY // Copy the framebuffer to the device context
        );
    }

    void Shutdown() {
        ReleaseDC(hwnd, hdc); // Release the device context
        DestroyWindow(hwnd); // Destroy the window
    }

    bool ProcessMessages() {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false; // Set running to false if a quit message is received
            }
            TranslateMessage(&msg); // Translate the message
            DispatchMessage(&msg); // Dispatch the message to the window procedure
        }
        return running; // Return the running status
    }

    std::vector<unsigned char>& GetFramebuffer() {
        return framebuffer; // Return a reference to the framebuffer
    }

    void Renderer::RestoreArea(int x, int y, int width, int height, const std::vector<unsigned char>& preRenderedLevel, int screenWidth, int screenHeight) {
        for (int py = y; py < y + height; ++py) {
            for (int px = x; px < x + width; ++px) {
                if (px >= 0 && px < screenWidth && py >= 0 && py < screenHeight) {
                    int index = (py * screenWidth + px) * 4; // RGBA format
                    int preRenderedIndex = (py * screenWidth + px) * 4;
                    framebuffer[index] = preRenderedLevel[preRenderedIndex]; // Red
                    framebuffer[index + 1] = preRenderedLevel[preRenderedIndex + 1]; // Green
                    framebuffer[index + 2] = preRenderedLevel[preRenderedIndex + 2]; // Blue
                    framebuffer[index + 3] = preRenderedLevel[preRenderedIndex + 3]; // Alpha
                }
            }
        }
    }
}