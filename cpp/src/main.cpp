#include "renderer.hpp"
#include "input.hpp"
#include "level.hpp"
#include <iostream>
#include <chrono>
#include "player.hpp"
#include <windows.h> 
#include <psapi.h>   

bool isColliding(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
    return !(x1 + width1 <= x2 || x2 + width2 <= x1 || y1 + height1 <= y2 || y2 + height2 <= y1);
}

int main() {
    // Initialize the renderer
    if (!Renderer::Init(1280, 720, "For The Queen")) {
        std::cerr << "Failed to initialize renderer\n";
        return 1;
    }

    // Load the level
    Level level;
    if (!level.Load("Level0.png", 1280, 720)) {
        std::cerr << "Failed to load PNG file\n";
        Renderer::Shutdown();
        return 1;
    }

    // Create a pre-rendered copy of the level
    std::vector<unsigned char> preRenderedLevel(1280 * 720 * 4, 255); // RGBA
    level.Render(preRenderedLevel, 1280, 720);

    // Load the player
    Player player;
    if (!player.LoadSpriteSheet("idle", "playerIDLE.png", 128, 32)) {
        std::cerr << "Failed to load idle sprite sheet\n";
        Renderer::Shutdown();
        return 1;
    }
    std::vector<Player::Frame> idleFrames = GenerateFrames(128, 32, 32, 32);
    player.AddAnimation("idle", "idle", idleFrames);

    if (!player.LoadSpriteSheet("run", "playerRUN.png", 512, 32)) {
        std::cerr << "Failed to load run sprite sheet\n";
        Renderer::Shutdown();
        return 1;
    }
    std::vector<Player::Frame> runFrames = GenerateFrames(512, 32, 32, 32);
    player.AddAnimation("run", "run", runFrames);

    player.PlayAnimation("idle");

    // Render the level into the framebuffer once
    Renderer::Clear();
    level.Render(Renderer::GetFramebuffer(), 1280, 720);

    // Define player dimensions and initial position
    int playerWidth = 32, playerHeight = 32; // Player's width and height
    int playerX = 375, playerY = 500; // Player's initial position on the screen
    int prevPlayerX = playerX, prevPlayerY = playerY; // Track the player's previous position
    float movementSpeed = 100.0f; // Player's movement speed in pixels per second
    float gravity = 800.0f; // Stronger gravity for faster descent
    float jumpVelocity = -400.0f; // Lower initial velocity for a shorter jump
    float playerVelocityY = 0.0f; // Current vertical velocity
    bool isOnGround = false; // Track whether the player is on the ground

    // Main game loop
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    bool isMoving = false; // Track whether the player is moving
    bool isFacingLeft = false; // Track the player's facing direction (true = left, false = right)

    while (Renderer::ProcessMessages()) {
        // Update input states for the current frame
        Input::Update();

        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();

        // Use a fixed time step for physics
        const float fixedDeltaTime = 1.0f / 60.0f;

        // Check if the Escape key is pressed to exit the game
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break; // Exit the loop if Escape is pressed
        }

        // Handle player movement based on key presses
        bool wasMoving = isMoving;
        isMoving = false;

        if (Input::IsKeyPressed(Input::Key::Left)) {
            int newX = playerX - static_cast<int>(movementSpeed * fixedDeltaTime);
            bool collision = false;
            isMoving = true;
            isFacingLeft = true;

            for (const auto& boundary : level.GetCollisionBoundaries()) {
                if (isColliding(newX, playerY, playerWidth, playerHeight,
                    boundary.x, boundary.y, boundary.width, boundary.height)) {
                    collision = true;
                    break;
                }
            }
            if (!collision) {
                playerX = newX;
            }
        }

        if (Input::IsKeyPressed(Input::Key::Right)) {
            int newX = playerX + static_cast<int>(movementSpeed * fixedDeltaTime);
            bool collision = false;
            isMoving = true;
            isFacingLeft = false;

            for (const auto& boundary : level.GetCollisionBoundaries()) {
                if (isColliding(newX, playerY, playerWidth, playerHeight,
                    boundary.x, boundary.y, boundary.width, boundary.height)) {
                    collision = true;
                    break;
                }
            }
            if (!collision) {
                playerX = newX;
            }
        }

        // Apply gravity
        playerVelocityY += gravity * fixedDeltaTime;

        // Calculate the new Y position
        int newY = playerY + static_cast<int>(playerVelocityY * fixedDeltaTime);

        // Check for collisions with all boundaries
        bool collision = false;
        for (const auto& boundary : level.GetCollisionBoundaries()) {
            if (isColliding(playerX, newY, playerWidth, playerHeight,
                boundary.x, boundary.y, boundary.width, boundary.height)) {
                collision = true;
                playerVelocityY = 0.0f; // Reset vertical velocity
                newY = boundary.y - playerHeight; // Align the player with the top of the boundary
                isOnGround = true;
                break;
            }
        }

        if (!collision) {
            isOnGround = false; // Player is no longer on the ground
        }
        playerY = newY;

        // Handle jumping
        if (Input::IsKeyPressed(Input::Key::Space) && isOnGround) {
            playerVelocityY = jumpVelocity; // Apply upward velocity
        }

        // Restore the area where the player was previously drawn
        Renderer::RestoreArea(prevPlayerX, prevPlayerY, playerWidth, playerHeight, preRenderedLevel, 1280, 720);

        // Update animations
        player.Update(static_cast<float>(fixedDeltaTime));
        if (isMoving && !wasMoving) {
            player.PlayAnimation("run");
        }
        else if (!isMoving && wasMoving) {
            player.PlayAnimation("idle");
        }

        // Draw the player sprite at the current position
        bool flipHorizontal = isFacingLeft;
        player.Render(playerX, playerY, Renderer::GetFramebuffer(), 1280, 720, flipHorizontal);

        // Present the rendered frame to the screen
        Renderer::Present();

        // Update the player's previous position
        prevPlayerX = playerX;
        prevPlayerY = playerY;

        lastFrameTime = currentTime;
    }

    // Clean up resources and shut down the renderer
    Renderer::Shutdown();
    return 0;
}