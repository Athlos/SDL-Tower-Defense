#pragma once

#include "label.h"

class Sprite;

class Button : public Label
{
	//Member Methods:
public:
	Button(std::string text);
	~Button();

	void Draw(BackBuffer& backBuffer);

	void SetCustomSprite(Sprite* sprite);

	void SetBackgroundColour(Uint8 r, Uint8 g, Uint8 b);

	Sprite* GetCustomSprite();

	//Member Data:
private:
	Sprite* m_sprite; // Sprite that can be placed instead of label

	bool m_customSprite; // Flag if button has a custom sprite instead of text

	SDL_Color m_backgroundColour; // Background colour
};

