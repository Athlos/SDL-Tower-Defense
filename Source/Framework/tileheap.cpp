#include "tileheap.h"
#include "tile.h"

TileHeap::TileHeap(int size)
	: m_size(size)
	, currentItemCount(0)
{
	m_tiles = new Tile*[size]; // Allocate array
}

TileHeap::~TileHeap()
{
	delete m_tiles;
	m_tiles = 0;
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
	bool sorted = false;

	while (!sorted)
	{
		int childIndexLeft = tile->m_heapIndex * 2 + 1;
		int childIndexRight = tile->m_heapIndex * 2 + 2;
		int swapIndex = 0;

		if (childIndexLeft < currentItemCount) // check left index is within the heap
		{
			swapIndex = childIndexLeft;

			if (childIndexRight < currentItemCount) // check right index is within the heap
			{
				if (m_tiles[childIndexLeft]->CompareTo(m_tiles[childIndexRight]) < 0) // Evaluate costs, check if left cost is smaller
				{
					swapIndex = childIndexRight;
				}
			}

			if (tile->CompareTo(m_tiles[swapIndex]) < 0) // check if cost is smaller than selected tile to swap
			{
				Swap(tile, m_tiles[swapIndex]); // if new index is smaller, swap with it and move down the heap
			}
			else
			{
				sorted = true; // If cost of tile is now smaller than its children, then it is now in the right place
			}
		}
		else
		{
			sorted = true; // Options are out of range, this is the bottom of the tree
		}
	}
}

void TileHeap::SortUp(Tile* tile)
{
	int parentIndex = (tile->m_heapIndex - 1) / 2;
	bool sorted = false;

	while (!sorted)
	{
		Tile* parent = m_tiles[parentIndex];

		if (tile->CompareTo(parent) > 0) // Keep swapping up
		{
			Swap(tile, parent);
		}
		else
		{
			sorted = false;
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
