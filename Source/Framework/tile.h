#pragma once

#include "position.h"

#include <vector>

class BackBuffer;
class Sprite;

enum TileState
{
	EMPTY,
	BLOCKED,
	FRINGE,
	EXPLORED,
	START,
	END,
	PATH
};

class Tile
{
	struct colour
	{
		int r;
		int g;
		int b;
	};

	//Member Methods:
public:
	Tile(int x, int y, TileState walkable);
	~Tile();

	int FCost() const;

	int GetHCost() const;
	int GetGCost() const;

	void Draw(BackBuffer& backBuffer);
	void Initialise(BackBuffer& backBuffer);

	int GetGridX() const;
	int GetGridY() const;

	Position GetCenter() const;
	
	int GetX() const;
	int GetY() const;

	int GetTileWidth() const;
	int GetTileHeight() const;

	void SetState(TileState state);
	TileState GetState() const;

	int CompareTo(Tile* tile);

	void SetGridSize(int width, int height);

	bool IsOccupied();
	void SetOccupied(bool occupied);

private:
	void SetSpriteDefault(Sprite* sprite);

	//Member Data:
public:
	int m_gCost;
	int m_hCost;

	Tile* m_parent;

	int m_heapIndex;

	int m_xScreenPos;
	int m_yScreenPos;

private:
	int m_gridX;
	int m_gridY;

	int m_gridWidth;
	int m_gridHeight;

	colour m_colour;
	TileState m_state;

	std::vector<Sprite*> m_tileSprites;

	bool m_occupied;
};

