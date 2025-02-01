#include "renderer.hpp"
#include "input.hpp"
#include "level.hpp"
#include <iostream>
#include <chrono>
#include "player.hpp"
#include <windows.h> // For performance monitoring
#include <psapi.h>   // For memory usage

//#pragma comment(lib, "Psapi.lib") 

// Function to get current memory usage (in MB)
/*double GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return static_cast<double>(pmc.PrivateUsage) / (1024 * 1024); // Convert bytes to MB
    }
    return 0.0;
}*/

// Function to get CPU usage (percentage)
/*double GetCPUUsage() {
    static FILETIME prevIdleTime = { 0 }, prevKernelTime = { 0 }, prevUserTime = { 0 };
    FILETIME idleTime, kernelTime, userTime;

    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return 0.0;
    }

    ULARGE_INTEGER idle, kernel, user;
    idle.LowPart = idleTime.dwLowDateTime;
    idle.HighPart = idleTime.dwHighDateTime;

    kernel.LowPart = kernelTime.dwLowDateTime;
    kernel.HighPart = kernelTime.dwHighDateTime;

    user.LowPart = userTime.dwLowDateTime;
    user.HighPart = userTime.dwHighDateTime;

    ULONGLONG idleDelta = idle.QuadPart - ((ULARGE_INTEGER{ prevIdleTime.dwLowDateTime, prevIdleTime.dwHighDateTime }).QuadPart);
    ULONGLONG kernelDelta = kernel.QuadPart - ((ULARGE_INTEGER{ prevKernelTime.dwLowDateTime, prevKernelTime.dwHighDateTime }).QuadPart);
    ULONGLONG userDelta = user.QuadPart - ((ULARGE_INTEGER{ prevUserTime.dwLowDateTime, prevUserTime.dwHighDateTime }).QuadPart);

    ULONGLONG total = kernelDelta + userDelta;
    double cpuUsage = total > 0 ? (1.0 - static_cast<double>(idleDelta) / total) * 100.0 : 0.0;

    prevIdleTime = idleTime;
    prevKernelTime = kernelTime;
    prevUserTime = userTime;

    return cpuUsage;
}*/

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

    // Load the idle sprite sheet (128x32, 32x32 frames)
    if (!player.LoadSpriteSheet("idle", "playerIDLE.png", 128, 32)) {
        std::cerr << "Failed to load idle sprite sheet\n";
        Renderer::Shutdown();
        return 1;
    }

    // Generate frames for the idle animation
    std::vector<Player::Frame> idleFrames = GenerateFrames(128, 32, 32, 32); // 4 frames (32x32 each)
    player.AddAnimation("idle", "idle", idleFrames);

    // Load the run sprite sheet (512x32, 32x32 frames)
    if (!player.LoadSpriteSheet("run", "playerRUN.png", 512, 32)) {
        std::cerr << "Failed to load idle sprite sheet\n";
        Renderer::Shutdown();
        return 1;
    }

    // Generate frames for the run animation
    std::vector<Player::Frame> runFrames = GenerateFrames(512, 32, 32, 32); // 16 frames (32x32 each)
    player.AddAnimation("run", "run", runFrames);

    // Start playing the idle animation
    player.PlayAnimation("idle");

    // Render the level into the framebuffer once
    Renderer::Clear();
    level.Render(Renderer::GetFramebuffer(), 1280, 720);

    // Define player dimensions and initial position
    int playerWidth = 32, playerHeight = 32; // Player's width and height
    int playerX = 375, playerY = 500; // Player's initial position on the screen
    int prevPlayerX = playerX; // Track the player's previous position
    float movementSpeed = 100.0f; // Player's movement speed in pixels per second

    // FPS counter variables
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    /*int frameCount = 0;
    double fps = 0.0;*/

    bool isMoving = false; // Track whether the player is moving
    bool isFacingLeft = false; // Track the player's facing direction (true = left, false = right)

    // Main game loop
    while (Renderer::ProcessMessages()) {
        // Update input states for the current frame
        Input::Update();

        // Check if the Escape key is pressed to exit the game
        if (Input::IsKeyPressed(Input::Key::Escape)) {
            break; // Exit the loop if Escape is pressed
        }

        // Handle player movement based on key presses
        bool wasMoving = isMoving;
        isMoving = false;

        // Handle player movement based on key presses
        if (Input::IsKeyPressed(Input::Key::Left)) {
            playerX -= static_cast<int>(movementSpeed * 0.016f); // Move player left
            isMoving = true;
            isFacingLeft = true;
        }
        if (Input::IsKeyPressed(Input::Key::Right)) {
            playerX += static_cast<int>(movementSpeed * 0.016f); // Move player right
            isMoving = true;
            isFacingLeft = false;
        }

        // Restore the level in the area where the player was previously drawn
        Renderer::RestoreArea(prevPlayerX, playerY, playerWidth, playerHeight, preRenderedLevel, 1280, 720);

        // Update the player animation
        auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();

        if (isMoving && !wasMoving) {
            player.PlayAnimation("run"); // Switch to running animation
        }
        else if (!isMoving && wasMoving) {
            player.PlayAnimation("idle"); // Switch back to idle animation
        }

        player.Update(static_cast<float>(deltaTime));

        // Draw the player sprite at the current position
        bool flipHorizontal = isFacingLeft; // Flip if moving left
        player.Render(playerX, playerY, Renderer::GetFramebuffer(), 1280, 720, flipHorizontal);

        // Update the player's previous position
        prevPlayerX = playerX;

        // Present the rendered frame to the screen
        Renderer::Present();

        lastFrameTime = currentTime;

        // FPS calculation
        /*auto currentTime = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastFrameTime).count();
        frameCount++;
        if (deltaTime >= 1.0) { // Calculate FPS every second
            fps = frameCount / deltaTime;
            frameCount = 0;
            lastFrameTime = currentTime;

            // Debug output: RAM, FPS, and CPU usage
            double ramUsage = GetMemoryUsage();
            double cpuUsage = GetCPUUsage();
            std::cout << "RAM: " << ramUsage << " MB, "
                << "FPS: " << fps << ", "
                << "CPU Usage: " << cpuUsage << "%\n";
        }*/
    }

    // Clean up resources and shut down the renderer
    Renderer::Shutdown();
    return 0; // Return 0 to indicate successful execution
}