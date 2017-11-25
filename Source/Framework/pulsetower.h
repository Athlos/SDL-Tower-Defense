#pragma once

#include "tower.h"

class Sprite;

class PulseTower : public Tower
{
	//Member Methods:
public:
	PulseTower();
	~PulseTower();

	bool Initialise(BackBuffer* backBuffer); // Create a tower

	void Process(float deltaTime); // Process a tower
	void Draw(BackBuffer& backBuffer); // Draw a tower

	void Shoot(); // Fire a projectile

	void SetScale(); // Scale sprites based on tile dimensions

	//Member Data:
public:
	AnimatedSprite* m_attackSprite; // Animated sprite of the tower's attack
};

