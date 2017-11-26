#pragma once

#include "SDL.h"

#include <map>
#include <vector>

class Label;
class Button;
class Sprite;
class BackBuffer;
class QuadTree;
class Sprite;

enum ColourTags // ID tags for the colour vector
{
	WHITE,
	BLACK,
	GRAY,
	RED,
	GREEN,
	BLUE,
	GOLD,
	DARKGREEN,
	DARKRED,
};

struct Icon // Basic icon struct to draw sprites on screen
{
	Sprite* image;
	bool drawable;

	Icon(Sprite* sprite, bool draw)
	{
		image = sprite;
		drawable = draw;
	}
};

class InterfaceManager
{
	//Member Methods:
public:
	InterfaceManager(BackBuffer* backbuffer);
	~InterfaceManager();

	void AddLabel(std::string tag, std::string text, int x, int y, int w, int h, ColourTags colourTag); // Adds a label to the manager

	Label* GetLabel(std::string tag); // Gets a label from the manager with a tag

	void AddButton(std::string tag, std::string text, std::string spriteLocation, int x, int y, int w, int h, ColourTags colourTag); // Adds a button to the manager

	Button* GetButton(std::string tag); // Gets a button from the manager with a tag

	void AddIcon(std::string tag, std::string spriteLocation, int x, int y, int w, int h); // Adds an icon to the manager

	Icon* GetIcon(std::string tag); // Gets an icon from the manager with a tag

	void Draw(); // Draws all ui elements

	SDL_Color GetColour(ColourTags tag); // Gets a colour from the list of colours in the game

	//Member Data:
protected:
	BackBuffer* m_backBuffer; // Backbuffer to render and create all UI elements

	std::map<std::string, Button*> m_buttons; // Map of buttons in game with their name for the key
	QuadTree* m_buttonCollisions; // Stores the bounds of the buttons in the quadtree

	std::map<std::string, Label*> m_labels; // Map of labels in game with their name for the key

	std::map<std::string, Icon*> m_icons; // Map of icons in game with their name for the key

	std::vector<SDL_Color> m_colours;
};