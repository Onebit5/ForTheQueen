#include "player.hpp"
#include "stb_image.h" // Include stb_image header
#include <iostream>

// Function to generate frames automatically
std::vector<Player::Frame> GenerateFrames(int spriteWidth, int spriteHeight, int frameWidth, int frameHeight, float defaultDuration) {
    std::vector<Player::Frame> frames;
    int columns = spriteWidth / frameWidth;
    int rows = spriteHeight / frameHeight;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            frames.push_back({ col * frameWidth, row * frameHeight, frameWidth, frameHeight, defaultDuration });
        }
    }
    return frames;
}

bool Player::LoadSpriteSheet(const std::string& name, const std::string& spritePath, int spriteWidth, int spriteHeight) {
    int x, y, n;
    unsigned char* data = stbi_load(spritePath.c_str(), &x, &y, &n, 4); // Request 4 channels (RGBA)
    if (!data) {
        std::cerr << "ERROR: Could not load sprite sheet: " << spritePath << "\n";
        return false;
    }

    SpriteSheet sheet;
    sheet.width = x;
    sheet.height = y;

    // Resize the sprite data to hold RGBA values
    sheet.data.resize(sheet.width * sheet.height * 4);

    // Copy the loaded data into spriteData
    std::memcpy(sheet.data.data(), data, sheet.data.size());

    stbi_image_free(data); // Free the image data after usage

    spriteSheets[name] = sheet; // Store the sprite sheet
    return true;
}

void Player::AddAnimation(const std::string& name, const std::string& spriteSheetName, const std::vector<Frame>& frames) {
    animations[name] = frames;
}

void Player::PlayAnimation(const std::string& name) {
    if (animations.find(name) != animations.end()) {
        currentAnimation = name;
        currentFrames = animations[name];
        currentFrameIndex = 0; // Reset to the first frame
        frameTimer = 0.0f;     // Reset the frame timer
    }
    else {
        std::cerr << "ERROR: Animation not found: " << name << "\n";
    }
}

void Player::Update(float deltaTime) {
    if (currentFrames.empty()) return;

    frameTimer += deltaTime;

    // Advance to the next frame if the elapsed time exceeds the current frame's duration
    while (frameTimer >= currentFrames[currentFrameIndex].duration) {
        frameTimer -= currentFrames[currentFrameIndex].duration;
        currentFrameIndex = (currentFrameIndex + 1) % currentFrames.size();
    }
}

void Player::Render(int x, int y, std::vector<unsigned char>& framebuffer, int screenWidth, int screenHeight, bool flipHorizontal) {
    if (currentFrames.empty()) {
        std::cerr << "ERROR: No frames to render\n";
        return;
    }

    const Frame& frame = currentFrames[currentFrameIndex];

    // Validate frame bounds
    auto it = spriteSheets.find(currentAnimation);
    if (it == spriteSheets.end()) {
        std::cerr << "ERROR: Sprite sheet not found for animation: " << currentAnimation << "\n";
        return;
    }

    const SpriteSheet& spriteSheet = it->second;
    if (frame.x < 0 || frame.y < 0 || frame.x + frame.width > spriteSheet.width || frame.y + frame.height > spriteSheet.height) {
        std::cerr << "ERROR: Frame out of sprite sheet bounds\n";
        return;
    }

    for (int py = 0; py < frame.height; ++py) {
        for (int px = 0; px < frame.width; ++px) {
            int srcX = flipHorizontal ? (frame.x + frame.width - 1 - px) : (frame.x + px); // Flip horizontally if needed
            int srcY = frame.y + py;

            int srcIndex = (srcY * spriteSheet.width + srcX) * 4; // Index in the sprite sheet
            int dstIndex = ((y + py) * screenWidth + (x + px)) * 4; // Index in the framebuffer

            // Validate framebuffer bounds
            if (x + px < 0 || x + px >= screenWidth || y + py < 0 || y + py >= screenHeight) continue;

            // Get the alpha value from the sprite sheet
            unsigned char alpha = spriteSheet.data[srcIndex + 3];

            // Blend the sprite with the framebuffer using alpha blending
            if (alpha > 0) { // Only draw if the pixel is not fully transparent
                framebuffer[dstIndex + 0] = spriteSheet.data[srcIndex + 2]; // Red
                framebuffer[dstIndex + 1] = spriteSheet.data[srcIndex + 1]; // Green
                framebuffer[dstIndex + 2] = spriteSheet.data[srcIndex + 0]; // Blue
                framebuffer[dstIndex + 3] = alpha; // Alpha
            }
        }
    }
}