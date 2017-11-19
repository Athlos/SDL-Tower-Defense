#include "building.h"
#include "tile.h"
#include "sprite.h"
#include "axisalignedboundingbox.h"
#include "backbuffer.h"

Building::Building(int baseCost)
	: m_cost(baseCost)
	, m_selected(false)
{
	Entity::Entity();
}

Building::~Building()
{
}

bool Building::Initialise(BackBuffer * backBuffer)
{
	return true;
}

void Building::Process(float deltaTime)
{
	Entity::Process(deltaTime);
}

void Building::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);

	if (m_selected)
	{
		m_selectionOutline->Draw(backBuffer);;
	}
}

BuildingType Building::GetType()
{
	return m_buildingType;
}

void Building::SetType(BuildingType type)
{
	m_buildingType = type;
}

void Building::SetTilePosition(Tile* tile)
{
	Entity::SetPosition(tile->GetX(), tile->GetY());
	m_tilePosition = tile;

	//Scale to tile size
	int tileWidth = m_tilePosition->GetTileWidth();
	int tileHeight = m_tilePosition->GetTileHeight();

	m_pSprite->SetWidth(tileWidth);
	m_pSprite->SetHeight(tileHeight);

	m_selectionOutline->SetX(m_x);
	m_selectionOutline->SetY(m_y);

	m_selectionOutline->SetWidth(tileWidth);
	m_selectionOutline->SetHeight(tileHeight);

	m_bounds = new AxisAlignedBoundingBox(m_pos, tileWidth / 2);
}

Tile* Building::GetTilePosition() const
{
	return m_tilePosition;
}

int Building::GetSellValue() const
{
	return 0;
}

int Building::GetCost() const
{
	return m_cost;
}

void Building::SetSelected(bool selected)
{
	m_selected = selected;
}