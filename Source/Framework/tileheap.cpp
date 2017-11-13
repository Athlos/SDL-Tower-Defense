#include "tileheap.h"
#include "tile.h"

TileHeap::TileHeap(int size)
{
	m_tiles = new Tile*[size]; // Allocate array
	m_size = size;
	currentItemCount = 0;
}

TileHeap::~TileHeap()
{
}

void TileHeap::Add(Tile* tile)
{
	tile->m_heapIndex = currentItemCount;

	m_tiles[currentItemCount] = tile;

	SortUp(tile);

	++currentItemCount;
}

Tile* TileHeap::RemoveFirst()
{
	Tile* first = m_tiles[0];
	--currentItemCount;

	m_tiles[0] = m_tiles[currentItemCount];
	m_tiles[0]->m_heapIndex = 0;

	SortDown(m_tiles[0]);

	return first;
}

bool TileHeap::Contains(Tile* tile)
{
	return (m_tiles[tile->m_heapIndex] == tile);
}

void TileHeap::SortDown(Tile* tile)
{
	while (true)
	{
		int childIndexLeft = tile->m_heapIndex * 2 + 1;
		int childIndexRight = tile->m_heapIndex * 2 + 2;
		int swapIndex = 0;

		if (childIndexLeft < currentItemCount)
		{
			swapIndex = childIndexLeft;

			if (childIndexRight < currentItemCount)
			{
				if (m_tiles[childIndexLeft]->CompareTo(m_tiles[childIndexRight]) < 0)
				{
					swapIndex = childIndexRight;
				}
			}

			if (tile->CompareTo(m_tiles[swapIndex]) < 0)
			{
				Swap(tile, m_tiles[swapIndex]);
			}
			else
			{
				return;
			}
		}
		else
		{
			return;
		}
	}
}

void TileHeap::SortUp(Tile* tile)
{
	int parentIndex = (tile->m_heapIndex - 1) / 2;

	while (true)
	{
		Tile* parent = m_tiles[parentIndex];

		if (tile->CompareTo(parent) > 0)
		{
			Swap(tile, parent);
		}
		else
		{
			break;
		}

		parentIndex = (tile->m_heapIndex - 1) / 2;
	}
}

void TileHeap::UpdateItem(Tile* tile)
{
	SortUp(tile);
}

void TileHeap::Swap(Tile* tileA, Tile* tileB)
{
	m_tiles[tileA->m_heapIndex] = tileB;
	m_tiles[tileB->m_heapIndex] = tileA;

	int tileIndex = tileA->m_heapIndex;

	tileA->m_heapIndex = tileB->m_heapIndex;
	tileB->m_heapIndex = tileIndex;
}

int TileHeap::Size()
{
	return currentItemCount;
}
