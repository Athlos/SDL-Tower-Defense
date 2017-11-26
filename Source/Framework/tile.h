#pragma once

#include "position.h"

#include <vector>

class BackBuffer;
class Sprite;

enum TileState // State of the tile
{
	//EMPTY,
	//BLOCKED,
	START,
	END,
	CLEAR
};

class Tile
{
	struct colour // TODO remove this and use standard colour stuff
	{
		int r;
		int g;
		int b;
	};

	//Member Methods:
public:
	Tile(int x, int y);
	~Tile();

	int FCost() const; // F cost for A* pathfinding

	int GetHCost() const; // H cost for A* pathfinding
	int GetGCost() const; // G cost for A* pathfinding

	void Draw(BackBuffer& backBuffer); // Draw the tile
	void Initialise(BackBuffer& backBuffer); // Create the tile

	int GetGridX() const; // Get the grid X coordinate
	int GetGridY() const; // Get the grid Y coordinate

	Position GetCenter() const; // Get the center of the tile
	
	int GetX() const; // Get X screen position
	int GetY() const; // Get Y screen position

	int GetTileWidth() const; // Get width of the tile
	int GetTileHeight() const; // Get height of the tile

	void SetState(TileState state); // Set the state of the tile
	TileState GetState() const; // Get the tile state

	int CompareTo(Tile* tile); // Compare to another tile

	void SetGridSize(int width, int height); // Set the width and height of the tile

	bool IsOccupied(); // Returns if the tile is occupied with an obstacle
	void SetOccupied(bool occupied); // Set if a tile is occupied

	void SetWall(bool hasWall); // Set if tile has a wall

private:
	void SetSpriteDefault(Sprite* sprite); // Sets default values for a sprite

	//Member Data:
public:
	int m_gCost; // Cost from start node for A* pathfinding
	int m_hCost; // Estimated heuristic cost to end node for A* pathfinding

	Tile* m_parent; // Neighbour tile that explored this one and set its heuristics

	int m_heapIndex; // Index of tile in the Tile heap

	int m_xScreenPos; // X position on screen
	int m_yScreenPos; // Y position on screen

private:
	int m_gridX; // X location in grid
	int m_gridY; // Y location in grid

	int m_gridWidth; // Width of the tile
	int m_gridHeight; // Height of the tile

	colour m_colour; // Colour to draw based on tile state
	TileState m_state; // State of the tile

	Sprite* m_tileSprite; // Ground sprite

	bool m_occupied; // Flag if tile is blocked
	bool m_hasWall; // Flag if tile has a wall
};

