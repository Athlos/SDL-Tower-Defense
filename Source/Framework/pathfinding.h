#pragma once

#include "tile.h"

#include <vector>
#include <queue>

class Grid;
class Tile;
class Position;

struct Comparator
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

	static Pathfinding& GetInstance();
	static void DestroyInstance();

	std::vector<Tile*> FindPath(int xStart, int yStart, int xEnd, int yEnd, bool drawPath);

	std::queue<Position*> SimplifyPath(std::vector<Tile*> path); // Turns path of tiles into positions, removes uneccessary points if direction does not change

	int GetDistance(Tile* a, Tile* b);
	int GetDistanceDiagonal(Tile* a, Tile* b);

	std::vector<Tile*> RetracePath(Tile* start, Tile* end, bool drawPath);

	void FindPathVector(int xStart, int yStart, int xEnd, int yEnd);
	void FindPathHeap(int xStart, int yStart, int xEnd, int yEnd);

	//Member Data
public:
	Grid* m_grid;

protected:
	static Pathfinding* sm_pInstance;
};

