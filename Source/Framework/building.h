#pragma once

#include "entity.h"

class Tile;

enum BuildingType
{
	NONE,
	WALL,
	TOWER
};

class Building : public Entity
{
	//Member Methods:
public:
	Building(int baseCost);
	~Building();

	virtual bool Initialise(BackBuffer* backBuffer);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	BuildingType GetType();
	void SetType(BuildingType type);

	void SetTilePosition(Tile* tile);
	Tile* GetTilePosition() const;

	int GetSellValue() const;
	int GetCost() const;

	void SetSelected(bool selected);

	//Member Data
protected:
	BuildingType m_buildingType;
	Tile* m_tilePosition;

	bool m_selected;
	BackBuffer* m_backBuffer;

	int m_cost;
	Sprite* m_selectionOutline;
};

