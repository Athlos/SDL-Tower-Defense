#pragma once

#include <string>
#include <vector>
#include <SDL_ttf.h>
#include <SDL.h>

class BackBuffer;
class SDL_Image;

enum Alignment
{
	LEFT,
	RIGHT,
	CENTER
};

class Label
{
	// Member Functions:
public:
	Label(std::string text, BackBuffer& backBuffer);
	Label();
	~Label();

	std::string GetText(); // Get the string of text stored
	void SetText(std::string textOnScreen); //only changes the string, does not make new image

	void Draw(BackBuffer& backBuffer); // Draw label

	SDL_Rect GetBounds(); // Get the collision bounds
	void SetBounds(int x, int y, int w, int h); // Set the bounds of your text, and the words will be constrained to this window

	void SetColour(int r, int g, int b, int a); // Set the colour, default is black already, set this before you set the text

	void SetFontSize(int size); // Sets the font size

	void SetTextAlignment(Alignment align); // Set text alignment

	void SetDrawable(bool draw); // Set if label will be drawn

protected:
	void ResizeText(); // Resizes the font size to find the best fit for the bounds
	void UpdateTexture(); // Creates a new texture for the text
	void UpdateFont(); // Reloads the font

	// Member Data:
protected:
	std::string m_text; // Label text
	BackBuffer* m_backBuffer; // Backbuffer to render and create textures

	SDL_Texture* m_textTexture; // Texture made from text

	SDL_Rect m_bounds; // Bounds of the label
	SDL_Rect m_currentBounds; // Current label bounds, used to resize the text appropriately within the bounds

	SDL_Color m_colour; // Text colour

	TTF_Font* m_font; // Font
	std::string m_fontName; // Filename of the font to use
	int m_fontSize; // Font size
	int m_maxFontSize; // Max font size to scale to

	Alignment m_textAlignment; // Alignment of the text

	bool m_drawable; // Flag if label will be drawn or clicked on
};

