#pragma once

#include "tower.h"

class SniperTower : public Tower
{
	//Member Methods:
public:
	SniperTower();
	~SniperTower();

	bool Initialise(BackBuffer* backBuffer); // Create a tower

	void Process(float deltaTime); // Process a tower
	void Draw(BackBuffer& backBuffer); // Draw a tower
	
	void SetCurrentTarget(Enemy* target); // Set the current target to shoot at
	void EvaluateTarget(); // Evaluate all available targets to choose a current one

	void SetEnemiesInRange(std::vector<Entity*> enemies); // Set the enemies in range from the quad tree

	void Shoot(); // Fire a projectile

	int GetTowerUpgradeCost() const; // Get cost to upgrade to next level

	//Member Data:
protected:
	Enemy* m_currentTarget; // Current enemy to shoot at
};

