#pragma once

#include "label.h"

class Sprite;

class Button : public Label
{
	//Member Methods:
public:
	Button(std::string text, BackBuffer& backBuffer);
	~Button();

	void Draw(BackBuffer& backBuffer); // Draw the button

	void SetCustomSprite(Sprite* sprite); // Set a sprite to draw instead of text for the button

	void SetBackgroundColour(SDL_Color colour); // Set the colour to draw as the button background
	void SetBackgroundColour(Uint8 r, Uint8 g, Uint8 b);// Set the colour to draw as the button background

	void SetBounds(int x, int y, int w, int h); // Set the bounds of your text, and the words will be constrained to this window

	bool WasClickedOn(int x, int y); // Check if button was clicked on, basic button functionality

	Sprite* GetCustomSprite(); // Get the sprite the button has set

	void SetActive(bool active); // Set flag if button should be flashing

	//Member Data:
private:
	Sprite* m_sprite; // Sprite that can be placed instead of label

	bool m_customSprite; // Flag if button has a custom sprite instead of text

	SDL_Color m_backgroundColour; // Background colour

	bool m_active; // Flag if button is active

	SDL_Color m_highlightColour; // Background colour

	SDL_Rect m_border; // Border bounds, takes the normal bounds and shrinks them down
	int m_borderWidth; // Border thickness in pixels

	SDL_Color m_borderColour; // Border colour, defaults as being a darker version of the background
};

