#include "renderer.hpp"
#include <windows.h>
#include <vector>

// Internal variables
HWND hwnd;
HDC hdc;
std::vector<unsigned char> framebuffer;
int screenWidth, screenHeight;
bool running = true;

// Window Procedure for processing events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        running = false;
        return 0;
    case WM_CLOSE:
        running = false;
        DestroyWindow(hwnd);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

namespace Renderer {
    bool Init(int width, int height, const char* title) {
        screenWidth = width;
        screenHeight = height;

        // Create framebuffer
        framebuffer.resize(screenWidth * screenHeight * 4, 255); // RGBA

        // Register Window Class
        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = "2D_Engine_Window";
        RegisterClass(&wc);

        // Create Window
        hwnd = CreateWindowEx(0, "2D_Engine_Window", title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,
            NULL, NULL, GetModuleHandle(NULL), NULL);

        if (!hwnd) return false;

        ShowWindow(hwnd, SW_SHOW);
        hdc = GetDC(hwnd);
        return true;
    }

    void Clear() {
        std::fill(framebuffer.begin(), framebuffer.end(), 255); // Fill with white
    }

    void DrawRect(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b) {
        for (int py = y; py < y + height; py++) {
            for (int px = x; px < x + width; px++) {
                if (px < 0 || px >= screenWidth || py < 0 || py >= screenHeight)
                    continue;

                int index = (py * screenWidth + px) * 4;
                framebuffer[index + 0] = r; // Red
                framebuffer[index + 1] = g; // Green
                framebuffer[index + 2] = b; // Blue
                framebuffer[index + 3] = 255; // Alpha
            }
        }
    }

    void Present() {
        BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = screenWidth;
        bmi.bmiHeader.biHeight = -screenHeight; // Negative to flip vertically
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        StretchDIBits(hdc, 0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight,
            framebuffer.data(), &bmi, DIB_RGB_COLORS, SRCCOPY);
    }

    void Shutdown() {
        ReleaseDC(hwnd, hdc);
        DestroyWindow(hwnd);
    }

    bool ProcessMessages() {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return running;
    }
}