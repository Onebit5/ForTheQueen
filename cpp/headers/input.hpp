#ifndef INPUT_H
#define INPUT_H

namespace Input {
	enum class Key { Escape, Left, Right };

	void Update(); // Updates the key states
	bool IsKeyPressed(Key key); // Checks if a key is pressed
}

#endif