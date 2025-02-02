#ifndef INPUT_HPP
#define INPUT_HPP

namespace Input { 
	enum class Key { Escape, Left, Right, Space }; // Enum class to define key states

	// Function to update the key states
	void Update(); 

	// Function to check if a specific key is pressed
	bool IsKeyPressed(Key key); 
}

#endif