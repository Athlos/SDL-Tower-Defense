#pragma once

#include "entity.h"

class Tile;

enum BuildingType // ID for different building types, used to tag the children of this class
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

	virtual bool Initialise(BackBuffer* backBuffer); // Create the building data by passing the backbuffer

	void Process(float deltaTime); // Process the building
	void Draw(BackBuffer& backBuffer); // Draw the building

	BuildingType GetType(); // Get the building tag
	void SetType(BuildingType type); // Set the building tab

	void SetTilePosition(Tile* tile); // Set the tile on the map this building is placed on
	Tile* GetTilePosition() const; // Get the tile on the map this building is placed on

	int GetSellValue() const; // Get the value this building has to sell
	int GetCost() const; // Get the cost of creating this building

	void SetSelected(bool selected); // Set the selected flag for this building

	//Member Data
protected:
	BuildingType m_buildingType; // The building type ID
	Tile* m_tilePosition; // The tile on the map this is placed on

	bool m_selected; // Selected tag to decide to draw the selection outline
	BackBuffer* m_backBuffer; // The backbuffer to create the sprites needed

	int m_cost; // Base cost to create the building
	Sprite* m_selectionOutline; // The sprite of the glowing outline for the building
};

