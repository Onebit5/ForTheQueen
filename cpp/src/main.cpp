#include "renderer.hpp"
#include "input.hpp"

int main() {
    // Initialize the renderer
    if (!Renderer::Init(1280, 720, "Engine")) {
        return -1;
    }

    // Main game loop
    while (Renderer::ProcessMessages()) {
        // Process input
        Input::Update();
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break;
        }

        // Clear the screen
        Renderer::Clear();

        // Draw a rectangle 
        Renderer::DrawRect(375, 500, 50, 100, 0, 0, 255); // Blue rectangle

        // Display the frame
        Renderer::Present();
    }

    // Clean up
    Renderer::Shutdown();
    return 0;
}