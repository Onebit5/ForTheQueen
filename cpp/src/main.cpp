#include "renderer.hpp"      
#include "input.hpp"         
#include "jsonLoader.hpp"    
#include "tilemap.hpp"       
#include "level.hpp"         
#include <iostream>          

int main() {
    // Initialize the renderer 
    if (!Renderer::Init(1280, 720, "Engine")) {
        return -1; // Return -1 if initialization fails
    }

    // Create a Level object to manage the game level
    Level level;
    // Load the level from a JSON file and a tileset image, specifying tile dimensions
    if (!level.Load("level0.json", "world_tileset.png", 16, 16)) {
        return -1; // Return -1 if level loading fails
    }

    // Define player dimensions and initial position
    int playerWidth = 50, playerHeight = 100; // Player's width and height
    int playerX = 375, playerY = 500;         // Player's initial position on the screen
    float movementSpeed = 200;                // Player's movement speed in pixels per second

    // Main game loop that continues until the window is closed
    while (Renderer::ProcessMessages()) {
        // Update input states for the current frame
        Input::Update();

        // Check if the Escape key is pressed to exit the game
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break; // Exit the loop if Escape is pressed
        }

        // Handle player movement based on key presses
        if (Input::IsKeyPressed(Input::Key::Left)) {
            std::cout << "Left key pressed" << std::endl; // Debug output for left movement
            playerX -= movementSpeed * .016f; // Move player left, simulating delta time
        }

        if (Input::IsKeyPressed(Input::Key::Right)) {
            std::cout << "Right key pressed" << std::endl; // Debug output for right movement
            playerX += movementSpeed * .016f; // Move player right, simulating delta time
        }

        // Clear the screen for the next frame
        Renderer::Clear();

        // Draw the player as a red rectangle at the current position
        Renderer::DrawRect(playerX, playerY, playerWidth, playerHeight, 255, 0, 0); // Red rectangle

        // Present the rendered frame to the screen
        Renderer::Present();
    }

    // Clean up resources and shut down the renderer
    Renderer::Shutdown();
    return 0; // Return 0 to indicate successful execution
}