#pragma once

#include "tile.h"

#include <vector>
#include <queue>

class Grid;
class Tile;
class Position;

struct Comparator // Comparator function used when testing std::makeHeap
{
public:

	bool operator() (const Tile* t1, const Tile* t2) const
	{
		return (t1->FCost() > t2->FCost() || (t1->FCost() == t2->FCost() && t1->GetHCost() > t2->GetHCost()));
	}
};

class Pathfinding
{
	//Member Methods:
public:
	Pathfinding();
	~Pathfinding();

	std::vector<Tile*> FindPath(int xStart, int yStart, int xEnd, int yEnd, bool drawPath); // Find a path from start to end on a grid. Uses a TileHeap

	std::queue<Position*> SimplifyPath(std::vector<Tile*> path); // Turns path of tiles into positions, removes uneccessary points if direction does not change

	int GetDistance(Tile* a, Tile* b); // Get distance between two tiles using manhattan distances

	std::vector<Tile*> RetracePath(Tile* start, Tile* end, bool drawPath); // Uses the closed set of the pathfinding algorithm to retrace from the start to end using a tile's parent

	void FindPathVector(int xStart, int yStart, int xEnd, int yEnd); // Finds the path from start to end, first iteration using a vector to hold the open set and iterates over all the elements to find the best choice, I have kept this for comparison reasons to see how it holds up compared to other implementations
	void FindPathHeap(int xStart, int yStart, int xEnd, int yEnd); // Finds the path from start to end, second iteration using std::make_heap and the associated functions. Had issue that heap kept being invalidated due to a poor comparator and had to keep being remade.

	//Member Data
public:
	Grid* m_grid; // Grid used to store the path
};

