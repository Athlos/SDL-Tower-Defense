#pragma once

#include "building.h"

#include <vector>

class Tile;
class Enemy;
class Projectile;
class AnimatedSprite;

enum TowerType
{
	SNIPER,
	PULSE
};

class Tower : public Building
{
	//Member Methods:
public:
	Tower(TowerType type);
	~Tower();

	bool Initialise(BackBuffer* backBuffer); // Create a tower

	void Process(float deltaTime); // Process a tower
	void Draw(BackBuffer& backBuffer); // Draw a tower

	void Shoot(); // Fire a projectile

	void SetTilePosition(Tile* tile); // Set the position of the tower with a tile

	void SetEnemiesInRange(std::vector<Entity*> enemies); // Set the enemies in range from the quad tree

	AxisAlignedBoundingBox* GetRangeBounds(); // Get the bounds for the tower range

	int GetTowerRange() const; // Get the range in tiles of the tower
	int GetTowerLevel() const; // Get the level of the tower
	int GetTowerDamage() const; // Get the damage of the tower
	float GetTowerFireRate() const; // Get the fire rate of the tower

	int GetSellValue() const; // Get value of the tower to sell

	virtual int GetTowerUpgradeCost() const; // Get cost to upgrade to next level
	void UpgradeTower(); // Upgrade teh tower to a new level

	bool IsMaxLevel(); // Returns if the tower has reached the final level of upgrades

	TowerType GetTowerType(); // Get which type of tower this is

	void SetScale(); // Scale sprites based on tile dimensions

	//Member Data:
protected:
	std::vector<Enemy*> m_targetsInRange; // All enemies in range

	int m_tileRange; // Range in tiles the tower can shoot
	float m_firingSpeed; // Seconds between shots of the tower
	float m_timeElapsed; // Time passed
	int m_damage; // Damage of the tower

	int m_currentLevel; // Current tower level
	int m_maxLevel; // Max level of upgrades

	AxisAlignedBoundingBox* m_towerRangeArea; // Area the tower can shoot enemies

	AnimatedSprite* m_towerSprite; // Animated sprite of the tower

	TowerType m_towerType; // The type of tower and attack it will do
};
