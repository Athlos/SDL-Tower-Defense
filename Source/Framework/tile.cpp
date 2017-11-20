#include "tile.h"
#include "backbuffer.h"
#include "position.h"
#include "sprite.h"

Tile::Tile(int x, int y, TileState walkable)
	: m_gridX(x)
	, m_gridY(y)
	, m_hCost(0)
	, m_gCost(0)
	, m_heapIndex(0)
	, m_hasWall(false)
{
	SetState(walkable);

	m_xScreenPos = m_gridX * m_gridWidth;
	m_yScreenPos = m_gridY * m_gridHeight;
}

Tile::~Tile()
{
}

int Tile::FCost() const
{
	return m_gCost + m_hCost;
}

int Tile::GetHCost() const
{
	return m_hCost;
}

int Tile::GetGCost() const
{
	return m_gCost;
}

void Tile::Draw(BackBuffer& backBuffer)
{
	backBuffer.DrawSprite(*m_tileSprite);

	if (m_state == START || m_state == END)
	{
		backBuffer.SetDrawColour(m_colour.r, m_colour.g, m_colour.b);

		backBuffer.DrawRectangle(m_xScreenPos, m_yScreenPos, m_xScreenPos + m_gridWidth, m_yScreenPos + m_gridHeight, 1);

		backBuffer.SetDrawColour(0, 0, 0);

		backBuffer.DrawRectangle(m_xScreenPos, m_yScreenPos, m_xScreenPos + m_gridWidth, m_yScreenPos + m_gridHeight, 0);
	}
}

void Tile::Initialise(BackBuffer& backBuffer)
{
	//Load wall base

	m_tileSprite = backBuffer.CreateSprite("assets\\ground_base.png");

	SetSpriteDefault(m_tileSprite);

}

int Tile::GetGridX() const
{
	return m_gridX;
}

int Tile::GetGridY() const
{
	return m_gridY;
}

Position Tile::GetCenter() const
{
	Position pos;

	pos.m_x = m_xScreenPos + (m_gridWidth / 2);
	pos.m_y = m_yScreenPos + (m_gridWidth / 2);

	return pos;
}

int Tile::GetX() const
{
	return m_xScreenPos;
}

int Tile::GetY() const
{
	return m_yScreenPos;
}

int Tile::GetTileWidth() const
{
	return m_gridWidth;
}

int Tile::GetTileHeight() const
{
	return m_gridHeight;
}

void Tile::SetState(TileState state)
{
	m_state = state;
	m_occupied = false;

	switch (m_state)
	{
	case EMPTY:
		m_colour.r = 255;
		m_colour.g = 255;
		m_colour.b = 255;
		break;
	case BLOCKED:
		m_colour.r = 0;
		m_colour.g = 0;
		m_colour.b = 0;

		m_occupied = true;
		break;
	case START:
		m_colour.r = 255;
		m_colour.g = 0;
		m_colour.b = 0;
		break;
	case END:
		m_colour.r = 0;
		m_colour.g = 255;
		m_colour.b = 0;
		break;
	}
}

TileState Tile::GetState() const
{
	return m_state;
}

int Tile::CompareTo(Tile* tile)
{
	int compare = FCost() - tile->FCost(); // Compare F cost first

	if (compare == 0)
	{
		compare = GetHCost() - tile->GetHCost(); // If F cost is equal, compare H cost
	}

	return -compare;
}

void Tile::SetGridSize(int width, int height)
{
	m_gridWidth = width;
	m_gridHeight = height;

	m_xScreenPos = m_gridX * m_gridWidth;
	m_yScreenPos = m_gridY * m_gridHeight;
}

bool Tile::IsOccupied()
{
	return m_occupied;
}

void Tile::SetOccupied(bool occupied)
{
	m_occupied = occupied;

	if (occupied)
	{
		m_state = BLOCKED;
	}
	else
	{
		m_state = EMPTY;
	}
}

void Tile::SetWall(bool hasWall)
{
	m_hasWall = hasWall;

	if (m_state == EMPTY)
	{
		m_state = BLOCKED;
	}
}

void Tile::SetSpriteDefault(Sprite* sprite)
{
	sprite->SetWidth(m_gridWidth);
	sprite->SetHeight(m_gridHeight);

	sprite->SetX(m_xScreenPos);
	sprite->SetY(m_yScreenPos);
}