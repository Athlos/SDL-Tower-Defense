#pragma once

#include "label.h"

class Sprite;

class Button : public Label
{
	//Member Methods:
public:
	Button(std::string text);
	~Button();

	void Draw(BackBuffer& backBuffer); // Draw the button

	void SetCustomSprite(Sprite* sprite); // Set a sprite to draw instead of text for the button

	void SetBackgroundColour(SDL_Color colour); // Set the colour to draw as the button background
	void SetBackgroundColour(Uint8 r, Uint8 g, Uint8 b);// Set the colour to draw as the button background

	Sprite* GetCustomSprite(); // Get the sprite the button has set

	//Member Data:
private:
	Sprite* m_sprite; // Sprite that can be placed instead of label

	bool m_customSprite; // Flag if button has a custom sprite instead of text

	SDL_Color m_backgroundColour; // Background colour
};

