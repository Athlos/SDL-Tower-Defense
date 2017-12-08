#include "interfacemanager.h"
#include "label.h"
#include "button.h"
#include "quadtree.h"
#include "game.h"
#include "backbuffer.h"
#include "sprite.h"
#include "logmanager.h"

#include <cassert>

InterfaceManager::InterfaceManager(BackBuffer* backbuffer)
{
	Position* topLeftPos = new Position(Game::SCREEN_HEIGHT / 2, Game::SCREEN_HEIGHT / 2);
	AxisAlignedBoundingBox* gridBounds = new AxisAlignedBoundingBox(topLeftPos, Game::SCREEN_HEIGHT / 2);

	m_buttonCollisions = new QuadTree(gridBounds);

	m_backBuffer = backbuffer;

	m_colours =
	{
		{ 255, 255, 255, 255 },
		{ 0, 0, 0, 255 },
		{ 192, 192, 192, 255 },
		{ 255, 0, 0, 255 },
		{ 0, 255, 0, 255 },
		{ 0, 0, 255, 255 },
		{ 255, 215, 0, 255 },
		{ 34, 139, 34, 255 },
		{ 178, 34, 34, 255 }
	}; // List of all colours in the game, list elements match the ColourTags
}

InterfaceManager::~InterfaceManager()
{
	//for (std::map<std::string, Label*>::iterator it = m_labels.begin(); it != m_labels.end(); ++it) // Draw Labels
	//{
	//	delete it->second;
	//}

	//for (std::map<std::string, Button*>::iterator it = m_buttons.begin(); it != m_buttons.end(); ++it) // Draw Buttons
	//{
	//	delete it->second;
	//}

	//for (std::map<std::string, Icon*>::iterator it = m_icons.begin(); it != m_icons.end(); ++it) // Draw Icons
	//{
	//	delete it->second;
	//}
}

void InterfaceManager::AddLabel(std::string tag, std::string text, int x, int y, int w, int h, ColourTags colourTag)
{
	Label* newLabel = new Label(text, *m_backBuffer);
	newLabel->SetBounds(x, y, w, h);
	newLabel->SetColour(m_colours[colourTag].r, m_colours[colourTag].g, m_colours[colourTag].b, m_colours[colourTag].a);

	m_labels[tag] = newLabel;
}

Label * InterfaceManager::GetLabel(std::string tag)
{
	if (m_labels.find(tag) == m_labels.end())
	{
		return 0;
	}

	return m_labels[tag];
}

void InterfaceManager::AddButton(std::string tag, std::string text, std::string spriteLocation, int x, int y, int w, int h, ColourTags colourTag)
{
	if (m_buttons.find(text) == m_buttons.end()) 
	{
		Button* newButton = new Button(text, *m_backBuffer);
		newButton->SetBounds(x, y, w, h);
		newButton->SetColour(m_colours[colourTag].r, m_colours[colourTag].g, m_colours[colourTag].b, m_colours[colourTag].a);

		if (spriteLocation != "")
		{
			Sprite* newSprite = m_backBuffer->CreateSprite(spriteLocation.c_str());

			assert(newSprite);

			newButton->SetCustomSprite(newSprite);
		}

		m_buttons[tag] = newButton;
	}
}

Button * InterfaceManager::GetButton(std::string tag)
{
	if (m_buttons.find(tag) == m_buttons.end())
	{
		return 0;
	}

	return m_buttons[tag];
}

void InterfaceManager::AddIcon(std::string tag, std::string spriteLocation, int x, int y, int w, int h)
{
	if (spriteLocation != "")
	{
		Sprite* newSprite = m_backBuffer->CreateSprite(spriteLocation.c_str());

		newSprite->SetX(x);
		newSprite->SetY(y);

		m_icons[tag] = new Icon(newSprite, true);
	}
}

Icon * InterfaceManager::GetIcon(std::string tag)
{
	if (m_icons.find(tag) == m_icons.end())
	{
		return 0;
	}

	return m_icons[tag];
}

void InterfaceManager::Draw()
{
	for (std::map<std::string, Label*>::iterator it = m_labels.begin(); it != m_labels.end(); ++it) // Draw Labels
	{
		it->second->Draw(*m_backBuffer);
	}

	for (std::map<std::string, Button*>::iterator it = m_buttons.begin(); it != m_buttons.end(); ++it) // Draw Buttons
	{
		it->second->Draw(*m_backBuffer);
	}

	for (std::map<std::string, Icon*>::iterator it = m_icons.begin(); it != m_icons.end(); ++it) // Draw Icons
	{
		if (it->second->drawable)
		{
			it->second->image->Draw(*m_backBuffer);
		}
	}
}

SDL_Color InterfaceManager::GetColour(ColourTags tag)
{
	return m_colours[tag];
}

void InterfaceManager::OnMouseClick(int x, int y)
{
	for (std::map<std::string, Button*>::iterator it = m_buttons.begin(); it != m_buttons.end(); ++it)
	{
		it->second->WasClickedOn(x, y);
	}
}

void InterfaceManager::OnMouseRelease(int x, int y)
{
	for (std::map<std::string, Button*>::iterator it = m_buttons.begin(); it != m_buttons.end(); ++it)
	{
		it->second->SetActive(false);
	}
}
