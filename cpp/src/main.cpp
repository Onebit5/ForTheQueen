#include "renderer.hpp"
#include "input.hpp"
#include "level.hpp"
#include <iostream>

int main() {
    // Initialize the renderer
    if (!Renderer::Init(1280, 720, "Engine")) {
        std::cerr << "Failed to initialize renderer\n";
        return 1;
    }

    // Load the level
    Level level;
    if (!level.Load("Level0.png")) {
        std::cerr << "Failed to load png file\n";
        return 1;
    }

    // Define player dimensions and initial position
    int playerWidth = 50, playerHeight = 100; // Player's width and height
    int playerX = 375, playerY = 500;         // Player's initial position on the screen
    float movementSpeed = 1500;                // Player's movement speed in pixels per second

    // Main game loop
    while (Renderer::ProcessMessages()) {
        // Update input states for the current frame
        Input::Update();

        // Check if the Escape key is pressed to exit the game
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break; // Exit the loop if Escape is pressed
        }

        // Handle player movement based on key presses
        if (Input::IsKeyPressed(Input::Key::Left)) {
            playerX -= movementSpeed * .016f; // Move player left, simulating delta time
        }
        if (Input::IsKeyPressed(Input::Key::Right)) {
            playerX += movementSpeed * .016f; // Move player right, simulating delta time
        }

        // Clear the screen for the next frame
        Renderer::Clear();

        // Render the level
        level.Render();

        // Draw the player as a red rectangle at the current position
        Renderer::DrawRect(playerX, playerY, playerWidth, playerHeight, 0, 0, 255); // Red rectangle

        // Present the rendered frame to the screen
        Renderer::Present();
    }

    // Clean up resources and shut down the renderer
    Renderer::Shutdown();

    return 0; // Return 0 to indicate successful execution
}