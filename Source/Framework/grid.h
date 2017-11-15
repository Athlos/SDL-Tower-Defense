#pragma once

#include <vector>

class Tile;
class BackBuffer;
class Pathfinding;

class Grid
{
	//Member Methods:
public:
	Grid(int x, int y);
	~Grid();

	void CreateGrid(BackBuffer& backBuffer); // Initialise and create a map grid

	void Draw(BackBuffer& backBuffer);

	std::vector<Tile*> GetNeighboursDiagonal(Tile* tile); // Get all the neighbouring tiles to a Tile including diagonal tiles
	std::vector<Tile*> GetNeighbours(Tile* tile); // Get all the neighbouring tiles to a Tile including diagonal tiles
	Tile* GetTileFromPixelCoord(int x, int y); // Get a tile based on screen coordinates
	Tile* GetTile(int x, int y); // Get a tile based on grid coordinates

	int GetGridSizeX(); // Returns the grid width
	int GetGridSizeY(); // Returns the grid height

	void ClearGridColours(); // Clears colours in grid

	Tile* GetGridStart(); // Returns the start of the grid, where enemies should spawn
	Tile* GetGridEnd(); // Returns the end point of the grid, where enemies should try to reach

	bool UpdatePath(); // Updates the path on the map for enemies, returns false if there is no path

	std::vector<Tile*> GetGridPath(); // Gets the path through the grind from gridStart to gridEnd

	//Member Data:
private:
	std::vector<std::vector<Tile*>*> m_grid; // 2D grid of tiles

	int m_gridSizeX; // grid x size
	int m_gridSizeY; // grid y size

	Pathfinding* m_pathFinding; // Used to get the path from start to end of the grid
	std::vector<Tile*> m_gridPath;
};

