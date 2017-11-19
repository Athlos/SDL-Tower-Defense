#pragma once

#include <string>
#include <vector>
#include <SDL_ttf.h>

#include "SDL.h"

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
	// Member Methods:
public:
	Label(std::string text);
	Label();
	~Label();

	std::string GetText(); // Get the string of text stored
	void SetText(std::string newText, BackBuffer& backBuffer); //Set the text image, if the text is the same it returns, you can call this in the process loop safely
	void SetText(std::string textOnScreen); //only changes the string, does not make new image

	void Draw(BackBuffer& backBuffer); // Draw label

	SDL_Rect GetBounds(); // Get the collision bounds
	void SetBounds(int x, int y, int w, int h); // Set the bounds of your text, and the words will be constrained to this window

	void SetColour(int r, int g, int b, int a); // Set the colour, default is black already, set this before you set the text

	bool WasClickedOn(int x, int y); // Check if label was clicked on, basic button functionality

	void SetFontSize(int size); // Sets the font size

	void SetTextAlignment(Alignment align); // Set text alignment

	void SetDrawable(bool draw); // Set if label will be drawn

protected:
	void ResizeText();

	// Member Data:
protected:
	std::string m_text; // Label text

	//Holds multi line message
	std::vector<std::string> m_textArray; // Multi line message
	SDL_Texture* m_textTexture; // Texture made from text

	SDL_Rect m_bounds; // Bounds of the label
	SDL_Rect m_currentBounds; // Current label bounds, used to resize the text appropriately within the bounds

	SDL_Color m_colour; // Text colour
	bool m_requiredUpdate; // Flag if text texture needs to be redone

	TTF_Font* m_font; // Font
	int m_fontSize; // Font size

	Alignment m_textAlignment; // Alignment of the text

	bool m_drawable; // Flag if label will be drawn or clicked on
};

