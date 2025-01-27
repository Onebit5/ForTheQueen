#include "input.hpp"
#include <Windows.h>

namespace Input {
	void Update() {
		// No-op for now, but could be used to track key states
	}

    bool IsKeyPressed(Key key) {
        switch (key) {
            case Key::Escape: return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
            case Key::Left:   return GetAsyncKeyState(VK_LEFT) & 0x8000;
            case Key::Right:  return GetAsyncKeyState(VK_RIGHT) & 0x8000;
        }
        return false;
    }
}