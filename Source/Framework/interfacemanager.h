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

enum ColourTags
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

struct Icon
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

	void AddLabel(std::string tag, std::string text, int x, int y, int w, int h, ColourTags colourTag);

	Label* GetLabel(std::string tag);

	void AddButton(std::string tag, std::string text, std::string spriteLocation, int x, int y, int w, int h, ColourTags colourTag);

	Button* GetButton(std::string tag);

	void AddIcon(std::string tag, std::string spriteLocation, int x, int y, int w, int h);

	Icon* GetIcon(std::string tag);

	void Draw();

	SDL_Color GetColour(ColourTags tag);

	//Member Data:
protected:
	BackBuffer* m_backBuffer;

	std::map<std::string, Button*> m_buttons;
	QuadTree* m_buttonCollisions;

	std::map<std::string, Label*> m_labels;

	std::map<std::string, Icon*> m_icons;

	std::vector<SDL_Color> m_colours = 
	{
		{255, 255, 255, 255},
		{0, 0, 0, 255},
		{192, 192, 192, 255},
		{255, 0, 0, 255},
		{0, 255, 0, 255},
		{0, 0, 255, 255},
		{ 255, 215, 0, 255 },
		{34, 139, 34, 255},
		{ 178, 34, 34, 255}
	};
};

//		m_highlighted->SetBounds(SCREEN_WIDTH * 0.76f, SCREEN_HEIGHT * 0.52f, SCREEN_WIDTH * 0.23f, SCREEN_HEIGHT * 0.07f);
//m_highlighted->SetColour(240, 230, 140, 50);