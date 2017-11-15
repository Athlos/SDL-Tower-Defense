#include "pathfinding.h"
#include "grid.h"
#include "tile.h"
#include "tileheap.h"
#include "position.h"

#include <queue>
#include <set>
#include <cassert>
#include <algorithm>

// Static Members:
Pathfinding* Pathfinding::sm_pInstance = 0;

Pathfinding::Pathfinding()
{
}

Pathfinding::~Pathfinding()
{
}

Pathfinding& Pathfinding::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Pathfinding();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void Pathfinding::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

std::vector<Tile*> Pathfinding::FindPath(int xStart, int yStart, int xEnd, int yEnd, bool drawPath)
{
	Tile* startTile = m_grid->GetTileFromPixelCoord(xStart, yStart);
	Tile* endTile = m_grid->GetTileFromPixelCoord(xEnd, yEnd);

	std::vector<Tile*> waypoints;

	TileHeap* openSet = new TileHeap(m_grid->GetGridSizeX() * m_grid->GetGridSizeY()); // Fringe set

	std::set<Tile*> closedSet; // Explored set

	openSet->Add(startTile); // Load start point

	while (openSet->Size() > 0)
	{
		Tile* currentTile = openSet->RemoveFirst();

		//Remove from fringe
		closedSet.insert(currentTile);

		//Update state
		if (drawPath)
		{
			//currentTile->SetState(EXPLORED);
		}

		if (currentTile == endTile) // Path found
		{
			waypoints = RetracePath(startTile, endTile, drawPath);
			delete(openSet);

			break;
		}

		for (int x = -1; x <= 1; ++x)
		{
			for (int y = -1; y <= 1; ++y)
			{
				if (x == 0 && y == 0)
				{
					continue;
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

						if (yNeighbour != 0)
						{
							if (yNeighbour->IsOccupied()) // If that neighbour is blocked, then you should not go diagonal
							{
								continue;
							}
						}

						Tile* xNeighbour = m_grid->GetTile(currentTile->GetGridX(), neighbour->GetGridY()); // Get the tile with the matching X axis to the current tile

						if (xNeighbour != 0)
						{
							if (xNeighbour->IsOccupied()) // If that neighbour is blocked, then you should not go diagonal
							{
								continue;
							}
						}
					}

					int newCostToNeighbour = currentTile->GetGCost() + GetDistanceDiagonal(currentTile, neighbour); // Calculate heuristic cost to move to that neighbour

					if (neighbour->GetGridX() != currentTile->GetGridX() && neighbour->GetGridY() != currentTile->GetGridY()) // Handle blocked diagonal routes
					{
						//neighbour is diagonal to current tile
						if (m_grid->GetTile(neighbour->GetGridX(), currentTile->GetGridY())->IsOccupied() && m_grid->GetTile(currentTile->GetGridX(), neighbour->GetGridY())->IsOccupied())
						{
							//Cannot go diagonal if there are blocked tiles around it
							continue;
						}
					}

					if (newCostToNeighbour < neighbour->GetGCost() || !openSet->Contains(neighbour))
					{
						neighbour->m_gCost = newCostToNeighbour;

						neighbour->m_hCost = GetDistanceDiagonal(neighbour, endTile);

						neighbour->m_parent = currentTile;

						if (!openSet->Contains(neighbour)) // If open set does not contain neighbour, push it
						{
							if (drawPath)
							{
								//neighbour->SetState(FRINGE);
							}

							openSet->Add(neighbour);
						}
					}
				}
			}
		}
	}

	return waypoints;
}

std::queue<Position*> Pathfinding::SimplifyPath(std::vector<Tile*> path)
{
	std::queue<Position*> simplePath;

	float oldDirX = 0;
	float oldDirY = 0;

	for (int i = 1; i < path.size(); ++i)
	{
		float dirX = path[i - 1]->GetCenter().m_x - path[i]->GetCenter().m_x;
		float dirY = path[i - 1]->GetCenter().m_y - path[i]->GetCenter().m_y;

		float hyp = sqrt(dirX*dirX + dirY*dirY);

		dirX /= hyp;
		dirY /= hyp;

		if (dirX != oldDirX || dirY != oldDirY)
		{
			simplePath.push(new Position(path[i - 1]->GetCenter().m_x, path[i - 1]->GetCenter().m_y));
		}

		oldDirX = dirX;
		oldDirY = dirY;
	}

	if (!path.empty())
	{
		simplePath.push(new Position(path[path.size() - 1]->GetCenter().m_x, path[path.size() - 1]->GetCenter().m_y));
	}

	return simplePath;
}

std::vector<Tile*> Pathfinding::RetracePath(Tile* start, Tile* end, bool drawPath)
{
	Tile* currentTile = end;

	std::vector<Tile*> path;

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

	//if (distX > distY)
	//{
	//	distance = 21 * distY + 10 * (distX - distY);
	//}
	//else
	//{
	//	distance = 21 * distX + 10 * (distY - distX);
	//}

	distance = 10 * (distX + distY);

	return distance;
}

int Pathfinding::GetDistanceDiagonal(Tile* a, Tile* b)
{
	int distX = abs(a->GetGridX() - b->GetGridX());
	int distY = abs(a->GetGridY() - b->GetGridY());

	int distance = 0;

	/*
	14 is diagonal value
	10 is horizontal or vertical value
	move diagonal first to close distance better
	then move horizontal or vertical
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

void Pathfinding::FindPathVector(int xStart, int yStart, int xEnd, int yEnd)
{
	Tile* startTile = m_grid->GetTileFromPixelCoord(xStart, yStart);
	Tile* endTile = m_grid->GetTileFromPixelCoord(xEnd, yEnd);

	std::vector<Tile*> openSet; // Fringe set
	std::set<Tile*> closedSet; // Explored set

	openSet.push_back(startTile); // Load start point

	while (openSet.size() > 0)
	{
		Tile* currentTile = openSet.at(0);
		int currentTileIndex = 0;

		//Find best Tile to explore
		for (int i = 1; i < openSet.size(); ++i)
		{
			if (openSet[i]->FCost() < currentTile->FCost() || openSet[i]->FCost() == currentTile->FCost() && openSet.at(i)->GetHCost() < currentTile->GetHCost())
			{
				currentTile = openSet.at(i);
				currentTileIndex = i;
			}
		}

		//Remove from fringe
		openSet.erase(openSet.begin() + currentTileIndex);
		closedSet.insert(currentTile);

		//Update state
		//currentTile->SetState(EXPLORED);

		if (currentTile == endTile) // Path found
		{
			RetracePath(startTile, endTile, false);
			return;
		}

		for each(Tile* neighbour in m_grid->GetNeighbours(currentTile)) // Process neighbouring tiles
		{
			if (neighbour->GetState() == BLOCKED || closedSet.find(neighbour) != closedSet.end()) // check if valid neighbour
			{
				continue;
			}

			int newCostToNeighbour = currentTile->GetGCost() + GetDistance(currentTile, neighbour);

			if (newCostToNeighbour < neighbour->GetGCost() || std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
			{
				neighbour->m_gCost = newCostToNeighbour;
				neighbour->m_hCost = GetDistance(neighbour, endTile);

				neighbour->m_parent = currentTile;

				if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end()) // If open set does not contain neighbour, push it
				{
					//neighbour->SetState(FRINGE);

					openSet.push_back(neighbour);
				}
			}
		}
	}
}

void Pathfinding::FindPathHeap(int xStart, int yStart, int xEnd, int yEnd)
{
	Tile* startTile = m_grid->GetTileFromPixelCoord(xStart, yStart);
	Tile* endTile = m_grid->GetTileFromPixelCoord(xEnd, yEnd);

	std::vector<Tile*> openSet; // Fringe set
	std::make_heap(openSet.begin(), openSet.end(), Comparator());

	std::set<Tile*> closedSet; // Explored set

	openSet.push_back(startTile); // Load start point
	std::push_heap(openSet.begin(), openSet.end(), Comparator());

	while (openSet.size() > 0)
	{
		std::make_heap(openSet.begin(), openSet.end(), Comparator());

		Tile* currentTile = openSet.front();

		std::pop_heap(openSet.begin(), openSet.end(), Comparator());
		openSet.pop_back();

		//Remove from fringe
		closedSet.insert(currentTile);

		//Update state
		//currentTile->SetState(EXPLORED);

		if (currentTile == endTile) // Path found
		{
			RetracePath(startTile, endTile, false);
			return;
		}

		for each(Tile* neighbour in m_grid->GetNeighbours(currentTile)) // Process neighbouring tiles
		{
			if (neighbour->GetState() == BLOCKED || closedSet.find(neighbour) != closedSet.end()) // check if valid neighbour
			{
				continue;
			}

			int newCostToNeighbour = currentTile->GetGCost() + GetDistance(currentTile, neighbour);

			if (newCostToNeighbour < neighbour->GetGCost() || std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
			{
				neighbour->m_gCost = newCostToNeighbour;
				neighbour->m_hCost = GetDistance(neighbour, endTile);

				neighbour->m_parent = currentTile;

				if (std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end()) // If open set does not contain neighbour, push it
				{
					//neighbour->SetState(FRINGE);

					if (!std::is_heap(openSet.begin(), openSet.end(), Comparator()))
						std::make_heap(openSet.begin(), openSet.end(), Comparator());

					openSet.push_back(neighbour);
					std::push_heap(openSet.begin(), openSet.end(), Comparator());
				}
			}
		}
	}
}
