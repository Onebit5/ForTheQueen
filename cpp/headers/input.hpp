#ifndef INPUT_H
#define INPUT_H

namespace Input { 
	enum class Key { Escape, Left, Right }; // Enum class to define key states

	// Function to update the key states
	void Update(); 

	// Function to check if a specific key is pressed
	bool IsKeyPressed(Key key); 
}

#endif