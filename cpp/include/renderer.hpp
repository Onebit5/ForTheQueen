#ifndef RENDERER_H
#define RENDERER_H

namespace Renderer {
	// Initializes the renderer with a window
	bool Init(int width, int height, const char* title);

	// Clears the screen
	void Clear();

	// Draws a rectangle
	void DrawRect(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b);

	// Displays the rendered frame
	void Present();

	// Shuts down the renderer
	void Shutdown();

	bool ProcessMessages();
}

#endif