#pragma once

#include <string>
#include <vector>
#include <SDL_ttf.h>

#include "SDL.h"

class BackBuffer;
class SDL_Image;

class Label
{
	// Member Methods:
public:
	Label(std::string text);
	~Label();

	//Set the text image, if the text is the same it returns, you can call this in the process loop safely
	void SetText(std::string newText, BackBuffer& backBuffer);

	//only changes the string, does not make new image
	void SetText(std::string textOnScreen);

	//standard draw
	void Draw(BackBuffer& backBuffer);

	//set the bounds of your text, and the words will be constrained to this window
	void SetBounds(int x, int y, int w, int h);

	//Set the colour, default is black already, set this before you set the text
	void SetColour(int r, int g, int b, int a);

	//get the string of text stored
	std::string GetText();

	//get the collision bounds
	SDL_Rect GetBounds();

	//Check if label was clicked on, basic button functionality
	bool WasClickedOn(int x, int y);

	// Member Data:
protected:
	std::string m_text;

	//Holds multi line message
	std::vector<std::string> m_textArray;
	SDL_Texture* m_textTexture;

	SDL_Rect m_bounds;
	SDL_Rect m_currentBounds;

	SDL_Color m_colour;
	bool m_requiredUpdate;

	TTF_Font* m_font;
	int m_fontSize;
};

