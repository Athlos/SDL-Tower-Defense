#pragma once

#include <string>
#include "SDL_ttf.h"
#include <SDL.h>   // All SDL App's need this

using namespace std;

// Forward Declarations:
class TextureManager;
class Sprite;
class AnimatedSprite;
struct SDL_Window;
struct SDL_Renderer;

class BackBuffer
{
	// Member Methods:
public:
	BackBuffer();
	~BackBuffer();

	bool Initialise(int width, int height); // Initialise the backbuffer with screen dimensions

	void Clear(); // Clear the buffer
	void Present(); // Present the buffer

	void SetClearColour(unsigned char r, unsigned char g, unsigned char b); // Set colour to clear the screen to

	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b); // Set the draw colour
	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a); // Set the draw colour with alpha

	void DrawSprite(Sprite& sprite); // Draw a sprite
	Sprite* CreateSprite(const char* pcFilename); // Create a sprite based on a filename

	void DrawRectangle(int x1, int y1, int x2, int y2, bool fill); // Draw a rectangle, either a frame or filled
	void DrawRectangle(SDL_Rect* bounds, bool fill); // Draw a rectangle, either a frame or filled

	void DrawLine(int x1, int y1, int x2, int y2); // Draw a line between 2 points

	void DrawText(SDL_Texture* textOnScreen, SDL_Rect bounds); // Draw text to the screen
	SDL_Texture* CreateText(std::string text, SDL_Color colour, TTF_Font* font); // Create a text texture

	void DrawAnimatedSprite(AnimatedSprite& sprite, SDL_Rect* bounds, SDL_Rect* raw); // Draw an animated sprite, pass in dimensions to draw to, and from
	AnimatedSprite* CreateAnimatedSprite(const char* pcFilename); // Create an animated sprite based on a filename

	void SetMouse(int x, int y); // Sets the mouse to these coordinates
	
protected:
	void LogSDLError(); // Log an error

private:
	BackBuffer(const BackBuffer& backBuffer);
	BackBuffer& operator=(const BackBuffer& backBuffer);

	// Member Data:
protected:
	TextureManager* m_pTextureManager; // Manages all textures requested

	// Fonts
	SDL_Window* m_pWindow; // Game window
	SDL_Renderer* m_pRenderer; // Game renderer
	SDL_Surface* m_surface; // Window surface

	int m_width; // Window width
	int m_height; // Window height

	unsigned char m_clearRed; // Red cleared channel
	unsigned char m_clearGreen; // Green cleared channel
	unsigned char m_clearBlue; // Blue cleared channel

};