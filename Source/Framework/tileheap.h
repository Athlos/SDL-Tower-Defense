#pragma once

#include <vector>

class Tile;

class TileHeap
{
	//Member Methods:
public:
	TileHeap(int size);
	~TileHeap();

	void Add(Tile* tile);
	Tile* RemoveFirst();
	
	bool Contains(Tile* tile);

	void SortDown(Tile* tile);
	void SortUp(Tile* tile);

	void UpdateItem(Tile* tile);

	void Swap(Tile* t1, Tile* t2);

	int Size();

	//Member Data:
private:
	Tile** m_tiles;

	int currentItemCount;

	int m_size;
};

