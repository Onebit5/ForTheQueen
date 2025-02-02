# 1. For The Queen Documentation (C++)

This project is a simple 2D game written in C++ that supports sprite-based animations, physics (gravity and jumping), and collision detection.

## Features

- **Sprite Animation**: Supports frame-by-frame animations with customizable frame durations.
- **Physics System**: Implements gravity and jumping mechanics.
- **Collision Detection**: Handles collisions between the player and level boundaries.
- **Renderer**: Renders sprites onto a framebuffer using RGBA pixel data.
- **Input Handling**: Processes keyboard input for movement and jumping.

## Dependencies

This project relies on the following external libraries:

1. **[stb_image.h](https://github.com/nothings/stb)**:
   - Used for loading PNG images as textures.
   - A single-header library for image loading, simplifying texture management.

2. **Standard Libraries**:
   - `<vector>`, `<unordered_map>`, `<string>`, `<chrono>`, etc., are used for data structures, timing, and string manipulation.

## Project Structure

The project is organized into several files, each responsible for a specific part of the game engine:

### Header Files
- **`player.hpp`**:
  - Defines the `Player` class, which handles animations, physics, and rendering for the player character.
  - Includes the `Frame` struct for defining individual animation frames.

- **`renderer.hpp`**:
  - Contains functions for initializing, rendering, and presenting the game framebuffer.

- **`input.hpp`**:
  - Handles keyboard input for controlling the player.

- **`level.hpp`**:
  - Manages the game level, including collision boundaries and pre-rendered level data.

### Source Files
- **`player.cpp`**:
  - Implements the `Player` class methods, including loading sprite sheets, playing animations, and updating physics.

- **`renderer.cpp`**:
  - Implements the rendering logic, including restoring areas of the framebuffer and drawing sprites.

- **`input.cpp`**:
  - Implements input handling for movement and jumping.

- **`level.cpp`**:
  - Implements level loading and rendering.

## How It Works

### 1. **Loading Sprite Sheets**
The `Player::LoadSpriteSheet` method uses `stb_image.h` to load PNG files into memory. Each sprite sheet is stored as an RGBA pixel array, which is then used for rendering.

### 2. **Generating Animation Frames**
The `GenerateFrames` function divides a sprite sheet into individual frames based on the specified frame width and height. Each frame includes a duration for smooth animations.

### 3. **Physics and Jumping**
The physics system applies gravity to the player's vertical velocity. When the player jumps, an initial upward velocity is applied, and gravity reduces it until the player falls back down. 

### 4. **Collision Detection**
Collisions are checked against predefined boundaries in the level. If a collision occurs, the player's position is adjusted, and their vertical velocity is reset.

### 5. **Rendering**
The `Player::Render` method draws the current animation frame onto the framebuffer. Alpha blending is used to handle transparency. 

## Building the Project

To compile and run the project, follow these steps:

### Install Dependencies:
- Ensure you have a C++ compiler (e.g., `g++`) installed.
- Download `stb_image.h` and place it in your project directory.

### Compile the Code:
Use the following command to compile the project:

```bash
g++ main.cpp player.cpp level.cpp renderer.cpp input.cpp -o forthequeen
```

### Run the game
```bash
./game_engine
```

# 2. For The Queen (Java)

A simple platformer game written in Java with manual rendering and physics handling.

## Features
- **Custom JSON Loader**: Loads level data from JSON files.
- **Manual Rendering**: No graphics library used; all rendering is handled manually.
- **Physics System**: Basic collision detection and movement.
- **Timer System**: Countdown timer per level with pause functionality.
- **Multiple Levels**: Transition between levels with state preservation fixes.

## Controls
- **Left Arrow (`←`)**: Move left
- **Right Arrow (`→`)**: Move right
- **Spacebar**: Jump

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/platformer-game.git
   cd platformer-game
   ```
2. Build the project:
   ```sh
   make   # If using a Makefile
   ```
   or manually compile:
   ```sh
   g++ -o game main.cpp -std=c++17
   ```
3. Run the game:
   ```sh
   ./game
   ```


# 3. For The Queen (Lua)

## Overview
This is a simple platformer game built using [LOVE2D](https://love2d.org/). The game features basic physics, platform collisions, and level transitions.

## Features
- Player movement (left, right, and jump)
- Collision detection with platforms
- Goal-based level transitions
- Rigidbody-based physics simulation

## Installation
### Prerequisites
- Install [LOVE2D](https://love2d.org/) (version 11.5 or later recommended)
- Ensure you have a working development environment for Lua

### Running the Game
1. Clone this repository:
   ```sh
   git clone https://github.com/yourusername/platformer-love2d.git
   cd platformer-love2d
   ```
2. Run the game using LOVE2D:
   ```sh
   love .
   ```

## Controls
- **Left Arrow** - Move Left
- **Right Arrow** - Move Right
- **Spacebar** - Jump

## Level System
- The player must reach the `Goal0` platform to transition to the next level.
- Levels are loaded dynamically using `loadLevel1()`.

## Fix for Auto Level Loading Issue
If the level loads immediately at start, ensure:
1. The player’s spawn position does not overlap with `Goal0`.
2. A `levelComplete` flag is used to prevent duplicate level transitions.

## Debugging
To visualize colliders, modify `love.draw()` to draw platform boundaries.
