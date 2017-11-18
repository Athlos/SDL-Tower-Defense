#pragma once

#include "building.h"

class Wall : public Building
{
	//Member Methods:
public:
	Wall(int baseCost);
	~Wall();

	bool Initialise(BackBuffer* backBuffer);

	void Draw(BackBuffer& backBuffer);

	int GetSellValue() const;

	//Member Data:
};

