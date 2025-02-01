#include "input.hpp"
#include <Windows.h>
#include <unordered_map>

namespace Input {
    static std::unordered_map<Key, bool> keyState; // Static map to store the state of keys (pressed or not)

    void Update() {
        // Update the state of the key based on its current status
        keyState[Key::Escape] = GetAsyncKeyState(VK_ESCAPE) & 0x8000;
        keyState[Key::Left] = GetAsyncKeyState(VK_LEFT) & 0x8000;
        keyState[Key::Right] = GetAsyncKeyState(VK_RIGHT) & 0x8000;
    }

    bool IsKeyPressed(Key key) {
        return keyState[key]; // Return the state of the specified key
    }
}