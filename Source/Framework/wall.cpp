#include "wall.h"
#include "backbuffer.h"
#include "sprite.h"

#include <cassert>

Wall::Wall(int baseCost) : Building(baseCost)
{
	m_buildingType = WALL;
}

Wall::~Wall()
{
}

bool Wall::Initialise(BackBuffer* backBuffer)
{
	m_pSprite = backBuffer->CreateSprite("assets\\wall_base.png");

	m_selectionOutline = backBuffer->CreateSprite("assets\\wall_selected.png");

	assert(m_pSprite);

	m_pSprite->SetX(m_x);
	m_pSprite->SetY(m_y);

	m_selectionOutline->SetX(m_x);
	m_selectionOutline->SetY(m_y);

	m_backBuffer = backBuffer;

	return true;
}

void Wall::Draw(BackBuffer & backBuffer)
{
	Building::Draw(backBuffer);
}

int Wall::GetSellValue() const
{
	return (float)m_cost * 0.5f;
}
