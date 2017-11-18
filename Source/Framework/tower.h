#pragma once

#include "entity.h"

#include <vector>

class Tile;
class Enemy;
class Projectile;
class AnimatedSprite;

struct Target
{
	Enemy* m_enemyTarget;
	float m_enemyDistance;

	Target(Enemy* enemy, float distance)
	{
		m_enemyTarget = enemy;
		m_enemyDistance = distance;
	}
};

class Tower : public Entity
{
	//Member Methods:
public:
	Tower(int range, float firingSpeed, int damage, int cost);
	~Tower();

	bool Initialise(BackBuffer* backBuffer);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void Shoot();

	void SetTilePosition(Tile* tile);
	Tile* GetTilePosition() const;

	void SetEnemiesInRange(std::vector<Entity*> enemies);

	AxisAlignedBoundingBox* GetRangeBounds();

	void SetCurrentTarget(Enemy* target);
	void EvaluateTarget();

	int GetTowerCost();

	int GetTowerRange() const;
	int GetTowerLevel() const;
	int GetTowerDamage() const;
	float GetTowerFireRate() const;

	int GetTowerValue() const;

	int GetTowerUpgradeCost() const;
	void UpgradeTower();

	bool IsMaxLevel();

	void SetSelected(bool selected);

	//Member Data:
protected:
	Tile* m_tilePosition;

	Enemy* m_currentTarget;

	std::vector<Enemy*> m_targetsInRange;

	int m_tileRange;
	float m_firingSpeed;
	float m_timeElapsed;
	int m_damage;
	int m_cost;

	int m_currentLevel;
	int m_maxLevel;

	bool m_selected;

	AxisAlignedBoundingBox* m_towerRangeArea;

	BackBuffer* m_backBuffer;

	AnimatedSprite* m_towerSprite;
	Sprite* m_selectionOutline;

};
