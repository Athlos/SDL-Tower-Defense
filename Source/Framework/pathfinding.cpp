#include "pathfinding.h"
#include "grid.h"
#include "tile.h"
#include "tileheap.h"
#include "position.h"
#include "logmanager.h"

#include <queue>
#include <set>
#include <cassert>
#include <algorithm>
#include <sstream>

Pathfinding::Pathfinding()
{
	m_grid = 0;
}

Pathfinding::~Pathfinding()
{
}

std::vector<Tile*> Pathfinding::FindPath(int xStart, int yStart, int xEnd, int yEnd, bool drawPath)
{
	Tile* startTile = m_grid->GetTileFromPixelCoord(xStart, yStart); // Convert start point to a tile
	Tile* endTile = m_grid->GetTileFromPixelCoord(xEnd, yEnd); // Convert end point to a tile

	assert(startTile);
	assert(endTile);

	std::vector<Tile*> waypoints = std::vector<Tile*>(); // Path

	TileHeap* openSet = new TileHeap(m_grid->GetGridSizeX() * m_grid->GetGridSizeY()); // Fringe set

	std::set<Tile*> closedSet = std::set<Tile*>(); // Explored set

	openSet->Add(startTile); // Load start point

	while (openSet->Size() > 0)
	{
		Tile* currentTile = openSet->RemoveFirst();

		//Remove from fringe
		closedSet.insert(currentTile);

		if (currentTile == endTile) // Path found
		{
			waypoints = RetracePath(startTile, endTile, drawPath); // Process the tiles to get the path	

			for each (Tile* t in closedSet)
			{
				t->m_gCost = 0;
				t->m_hCost = 0;
			}

			break;
		}

		//Get the neighbours
		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				if (x == 0 && y == 0)
				{
					continue; // Skip current
				}

				Tile* neighbour = m_grid->GetTile(currentTile->GetGridX() + x, currentTile->GetGridY() + y);

				if (neighbour != 0)
				{
					//We have found a valid neighbour, lets try to evaluate it

					if (neighbour->IsOccupied() || closedSet.find(neighbour) != closedSet.end()) //Check if neighbour can be evaluated by checking if you can walk on it and if its not already in the closed set
					{
						continue;
					}

					//Now we want to check if this tile is in a diagonal direction
					//If it is, you can only move to it if the path does not cut past an occupied tile
					//Diagonal tiles have a different X and Y coordinate
					if (currentTile->GetGridX() != neighbour->GetGridX() && currentTile->GetGridY() != neighbour->GetGridY())
					{
						//If this is a diagonal tile, we need to check if it has a blocked neighbour that we would cut across if we went this way
						Tile* yNeighbour = m_grid->GetTile(neighbour->GetGridX(), currentTile->GetGridY()); // Get the tile with the matching Y axis to the current tile
						Tile* xNeighbour = m_grid->GetTile(currentTile->GetGridX(), neighbour->GetGridY()); // Get the tile with the matching X axis to the current tile

						//There should never be a situation where either neighbour is 0 when the diagonal is valid
						assert(yNeighbour);
						assert(xNeighbour);

						if (yNeighbour->IsOccupied() || xNeighbour->IsOccupied()) // If either neighbour is blocked, then you should not go diagonal
						{
							continue;
						}
					}

					int newCostToNeighbour = currentTile->GetGCost() + GetDistance(currentTile, neighbour); // Calculate heuristic cost to move to that neighbour

					if (newCostToNeighbour <= neighbour->GetGCost() || !openSet->Contains(neighbour))
					{
						neighbour->m_gCost = newCostToNeighbour;

						neighbour->m_hCost = GetDistance(neighbour, endTile);

						neighbour->m_parent = currentTile;

						if (!openSet->Contains(neighbour)) // If open set does not contain neighbour, push it
						{
							neighbour->m_heapIndex = -1;
							openSet->Add(neighbour);
						}
					}
				}
			}
		}
	}

	delete(openSet);
	openSet = 0;

	return waypoints;
}

std::queue<Position*> Pathfinding::SimplifyPath(std::vector<Tile*> path)
{
	std::queue<Position*> simplePath; // Queue of positions on map to move through

	float oldDirX = 0;
	float oldDirY = 0;

	for (int i = 1; i < path.size(); ++i) // Go over all tiles in the path
	{
		//Get the direction from last tile to current tile
		float dirX = path[i - 1]->GetCenter().m_x - path[i]->GetCenter().m_x;
		float dirY = path[i - 1]->GetCenter().m_y - path[i]->GetCenter().m_y;

		float hyp = sqrt(dirX*dirX + dirY*dirY);

		dirX /= hyp;
		dirY /= hyp;

		//If its different to the previous direction, then it becomes a new waypoint
		if (dirX != oldDirX || dirY != oldDirY)
		{
			simplePath.push(new Position(path[i - 1]->GetCenter().m_x, path[i - 1]->GetCenter().m_y));
		}

		oldDirX = dirX;
		oldDirY = dirY;
	}

	if (!path.empty()) // Push last point
	{
		simplePath.push(new Position(path[path.size() - 1]->GetCenter().m_x, path[path.size() - 1]->GetCenter().m_y));
	}

	return simplePath;
}

std::vector<Tile*> Pathfinding::RetracePath(Tile* start, Tile* end, bool drawPath)
{
	Tile* currentTile = end;

	std::vector<Tile*> path = std::vector<Tile*>();

	while (currentTile != start)
	{
		if (drawPath)
		{
			//currentTile->SetState(PATH);
		}

		path.push_back(currentTile);

		currentTile = currentTile->m_parent;
	}

	path.push_back(start);

	std::reverse(path.begin(), path.end());

	return path;
}

int Pathfinding::GetDistance(Tile* a, Tile* b)
{
	int distX = abs(a->GetGridX() - b->GetGridX());
	int distY = abs(a->GetGridY() - b->GetGridY());

	int distance = 0;

	/*
	14 is diagonal value
	10 is horizontal or vertical value
	move diagonal first to close distance better
	then move horizontal
	*/
	if (distX > distY)
	{
		distance = 14 * distY + 10 * (distX - distY);
	}
	else
	{
		distance = 14 * distX + 10 * (distY - distX);
	}

	return distance;
}