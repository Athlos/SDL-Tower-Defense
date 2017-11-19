#include "interfacemanager.h"
#include "label.h"
#include "button.h"
#include "quadtree.h"
#include "game.h"
#include "backbuffer.h"
#include "sprite.h"

InterfaceManager::InterfaceManager(BackBuffer* backbuffer)
{
	Position* topLeftPos = new Position(Game::SCREEN_HEIGHT / 2, Game::SCREEN_HEIGHT / 2);
	AxisAlignedBoundingBox* gridBounds = new AxisAlignedBoundingBox(topLeftPos, Game::SCREEN_HEIGHT / 2);

	m_buttonCollisions = new QuadTree(gridBounds);

	m_backBuffer = backbuffer;
}

InterfaceManager::~InterfaceManager()
{
}

void InterfaceManager::AddLabel(std::string tag, std::string text, int x, int y, int w, int h, ColourTags colourTag)
{
	Label* newLabel = new Label(text);
	newLabel->SetBounds(x, y, w, h);
	newLabel->SetColour(m_colours[colourTag].r, m_colours[colourTag].g, m_colours[colourTag].b, m_colours[colourTag].a);

	m_labels[tag] = newLabel;
}

Label * InterfaceManager::GetLabel(std::string tag)
{
	return m_labels[tag];
}

void InterfaceManager::AddButton(std::string tag, std::string text, std::string spriteLocation, int x, int y, int w, int h, ColourTags colourTag)
{
	Button* newButton = new Button(text);
	newButton->SetBounds(x, y, w, h);
	newButton->SetColour(m_colours[colourTag].r, m_colours[colourTag].g, m_colours[colourTag].b, m_colours[colourTag].a);

	if (spriteLocation != "")
	{
		Sprite* newSprite = m_backBuffer->CreateSprite(spriteLocation.c_str());
		newButton->SetCustomSprite(newSprite);
	}

	m_buttons[tag] = newButton;
}

Button * InterfaceManager::GetButton(std::string tag)
{
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
