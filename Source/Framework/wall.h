#pragma once

#include "building.h"

class Wall : public Building
{
	//Member Methods:
public:
	Wall(int baseCost);
	~Wall();

	bool Initialise(BackBuffer* backBuffer); // Create a wall

	void Draw(BackBuffer& backBuffer); // Draw the wall

	int GetSellValue() const; // Get the value to sell

	//Member Data:
};

