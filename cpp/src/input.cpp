#include "input.hpp"
#include <Windows.h>
#include <unordered_map>
#include <iostream>

static std::unordered_map<int, bool> previousKeyStates;

void Input::Update() {
    // Update previous states after checking the current state
    for (int i = 0; i < 256; ++i) { 
        bool currentState = GetAsyncKeyState(i) & 0x8000;
        if (previousKeyStates.find(i) == previousKeyStates.end()) {
            previousKeyStates[i] = false; // Initialize if not already present
        }
        previousKeyStates[i] = currentState;
    }
}

bool Input::IsKeyPressed(Key key) {
    int vkCode = 0;
    switch (key) {
    case Key::Left: vkCode = VK_LEFT; break;
    case Key::Right: vkCode = VK_RIGHT; break;
    case Key::Escape: vkCode = VK_ESCAPE; break;
    case Key::Space: vkCode = VK_SPACE; break;
    default: return false;
    }
    return GetAsyncKeyState(vkCode) & 0x8000;
}