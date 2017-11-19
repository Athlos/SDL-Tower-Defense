#include "grid.h"
#include "tile.h"
#include "backbuffer.h"
#include "game.h"
#include "pathfinding.h"
#include "position.h"

#include <math.h>
#include <queue>

Grid::Grid(int x, int y)
{
	m_gridSizeX = x;
	m_gridSizeY = y;

	m_pathFinding = new Pathfinding();
	m_pathFinding->m_grid = this;
}

Grid::~Grid()
{
	for (int x = 0; x < m_gridSizeX; ++x)
	{
		delete(m_grid.at(x));
	}
}

void Grid::CreateGrid(BackBuffer& backBuffer)
{
	//Create empty grid
	for (int x = 0; x < m_gridSizeX; ++x)
	{
		m_grid.push_back(new std::vector<Tile*>);
		for (int y = 0; y < m_gridSizeY; ++y)
		{
			Tile* newTile = new Tile(x, y, EMPTY);
			newTile->SetGridSize((Game::GetInstance().SCREEN_WIDTH * 0.75f) / m_gridSizeX, Game::GetInstance().SCREEN_HEIGHT / m_gridSizeY);
			newTile->Initialise(backBuffer);

			m_grid.at(x)->push_back(newTile);
		}
	}

	m_grid.at(0)->at(0)->SetState(START);
	m_grid.at(m_gridSizeX - 1)->at(m_gridSizeY - 1)->SetState(END);

	UpdatePath();
}

void Grid::Draw(BackBuffer& backBuffer)
{
	for (int x = 0; x < m_gridSizeX; ++x)
	{
		for (int y = 0; y < m_gridSizeY; ++y)
		{
			m_grid.at(x)->at(y)->Draw(backBuffer);
		}
	}

	//Draw Map Path
	//backBuffer.SetDrawColour(0, 255, 0);

	//std::queue<Position*> path = m_pathFinding->SimplifyPath(m_gridPath);

	//Position* current = 0;
	//Position* next = 0;

	//if (!path.empty())
	//{
	//	current = path.front();
	//	path.pop();
	//}

	//while (!path.empty())
	//{
	//	next = path.front();
	//	path.pop();

	//	backBuffer.DrawLine(current->m_x, current->m_y, next->m_x, next->m_y);

	//	delete current;

	//	current = next;
	//}

	//delete next;
}

std::vector<Tile*> Grid::GetNeighboursDiagonal(Tile* tile)
{
	std::vector<Tile*> neighbours;

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}

			int checkX = tile->GetGridX() + x;
			int checkY = tile->GetGridY() + y;

			if (checkX >= 0 && checkX < m_gridSizeX && checkY >= 0 && checkY < m_gridSizeY) // Check is valid position
			{
				neighbours.push_back(m_grid.at(checkX)->at(checkY));
			}
		}
	}

	return neighbours;
}

std::vector<Tile*> Grid::GetNeighbours(Tile* tile)
{
	std::vector<Tile*> neighbours;

	for (int offset = -1; offset <= 1; ++offset)
	{
		if (offset == 0)
		{
			continue;
		}

		int checkX = tile->GetGridX() + offset;
		int checkY = tile->GetGridY() + offset;

		if (checkX >= 0 && checkX < m_gridSizeX) // Check is valid x position
		{
			neighbours.push_back(m_grid.at(checkX)->at(tile->GetGridY()));
		}

		if (checkY >= 0 && checkY < m_gridSizeY) // Check is valid y position
		{
			neighbours.push_back(m_grid.at(tile->GetGridX())->at(checkY));
		}
	}

	return neighbours;
}

Tile* Grid::GetTileFromPixelCoord(int x, int y)
{
	float xPos = x / (float)((Game::GetInstance().SCREEN_WIDTH * 0.75f) / m_gridSizeX);
	float yPos = y / (float)(Game::GetInstance().SCREEN_HEIGHT / m_gridSizeY);

	if (xPos >= 0 && xPos < m_gridSizeX && yPos >= 0 && yPos < m_gridSizeY)
	{
		return m_grid.at(xPos)->at(yPos);
	}
	else
	{
		return 0;
	}
}

Tile* Grid::GetTile(int x, int y)
{
	if (x >= 0 && x < m_gridSizeX && y >= 0 && y < m_gridSizeY)
	{
		return m_grid.at(x)->at(y);
	}
	else
	{
		return 0;
	}
}

int Grid::GetGridSizeX()
{
	return m_gridSizeX;
}

int Grid::GetGridSizeY()
{
	return m_gridSizeY;
}

void Grid::ClearGridColours()
{
	for (int x = 0; x < m_gridSizeX; ++x)
	{
		for (int y = 0; y < m_gridSizeY; ++y)
		{
			if (m_grid.at(x)->at(y)->GetState() != BLOCKED)
			{
				m_grid.at(x)->at(y)->SetState(EMPTY);
			}
		}
	}
}

Tile* Grid::GetGridStart()
{
	return m_grid.at(0)->at(0);
}

Tile* Grid::GetGridEnd()
{
	return m_grid.at(m_gridSizeX - 1)->at(m_gridSizeY - 1);
}

bool Grid::UpdatePath()
{
	std::vector<Tile*> newPath = m_pathFinding->FindPath(GetGridStart()->GetCenter().m_x, GetGridStart()->GetCenter().m_y, GetGridEnd()->GetCenter().m_x, GetGridEnd()->GetCenter().m_y, 0);

	if (!newPath.empty())
	{
		m_gridPath = newPath;
	}

	return !newPath.empty();
}

std::vector<Tile*> Grid::GetGridPath()
{
	return m_gridPath;
}