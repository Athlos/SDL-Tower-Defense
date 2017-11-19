#pragma once

#include <vector>

class Tile;

class TileHeap
{
	//Member Methods:
public:
	TileHeap(int size);
	~TileHeap();

	void Add(Tile* tile); // Add a tile to the heap
	Tile* RemoveFirst(); // Remove element at top of heap
	
	bool Contains(Tile* tile); // Check if heap contains a tile

	void SortDown(Tile* tile); // Sort a tile down the heap
	void SortUp(Tile* tile); // Sort a tile up the heap

	void UpdateItem(Tile* tile); // Update the state of an item in the heap

	void Swap(Tile* t1, Tile* t2); // Swap tiles in the heap

	int Size(); // Get size of the heap

	//Member Data:
private:
	Tile** m_tiles; // Array of Tile* for the heap

	int currentItemCount; // Current amount of tiles in the heap

	int m_size; // Sizeo of the heap
};

