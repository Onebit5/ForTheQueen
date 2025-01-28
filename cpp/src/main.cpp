#include "renderer.hpp"
#include "input.hpp"

int main() {
    // Initialize the renderer
    if (!Renderer::Init(1280, 720, "Engine")) {
        return -1;
    }

    // Player variables
    int playerWidth = 50, playerHeight = 100;
    int playerX = 375, playerY = 500;
    int movementSpeed = 200;

    // Main game loop
    while (Renderer::ProcessMessages()) {
        // Process input
        Input::Update();

        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break;
        }

        // Handle player movement
        if (Input::IsKeyPressed(Input::Key::Left)) {
            playerX -= movementSpeed * .016f; // DeltaTime simulation
        }

        if (Input::IsKeyPressed(Input::Key::Right)) {
            playerX += movementSpeed * .016f; // DeltaTime simulation
        }


        // Clear the screen
        Renderer::Clear();

        // Draw a rectangle 
        Renderer::DrawRect(playerX, playerY, playerWidth, playerHeight, 255, 0, 0); // Blue rectangle

        // Display the frame
        Renderer::Present();
    }

    // Clean up
    Renderer::Shutdown();
    return 0;
}