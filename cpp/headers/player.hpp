#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <string>
#include <unordered_map>

class Player {
public:
    struct Frame {
        int x, y, width, height; // Position and size of the frame in the sprite sheet
        float duration; // Duration of the frame in seconds
    };

    bool LoadSpriteSheet(const std::string& name, const std::string& spritePath, int spriteWidth, int spriteHeight);
    void AddAnimation(const std::string& name, const std::string& spriteSheetName, const std::vector<Frame>& frames);
    void PlayAnimation(const std::string& name);
    void Update(float deltaTime);
    void Render(int x, int y, std::vector<unsigned char>& framebuffer, int screenWidth, int screenHeight, bool flipHorizontal);

private:
    struct SpriteSheet {
        std::vector<unsigned char> data; // RGBA pixel data for the sprite sheet
        int width, height;               // Dimensions of the sprite sheet
    };

    std::unordered_map<std::string, SpriteSheet> spriteSheets; // Map of loaded sprite sheets
    std::unordered_map<std::string, std::vector<Frame>> animations; // Map of animations
    std::string currentAnimation; // Name of the currently playing animation
    std::vector<Frame> currentFrames; // Frames of the currently playing animation
    int currentFrameIndex = 0; // Index of the current frame
    float frameTimer = .0f; // Time elapsed since the last frame change
};

// Free utility function to generate frames
std::vector<Player::Frame> GenerateFrames(int spriteWidth, int spriteHeight, int frameWidth, int frameHeight, float defaultDuration = .1f);

#endif