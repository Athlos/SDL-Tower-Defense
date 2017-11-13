#pragma once

#include "entity.h"

#include <vector>

class Tile;
class Enemy;

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
	Tower(int range, float firingSpeed, int damage);
	~Tower();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void Shoot();

	void SetTilePosition(Tile* tile);

	void GetEnemiesInRange(std::vector<Enemy*> enemies);
	void SetCurrentTarget(Target* target);

	bool HasValidTarget();

	//Member Data:
protected:
	Tile* m_tilePosition;

	Target* m_currentTarget;

	std::vector<Enemy*> m_tagetsInRange;

	int m_tileRange;
	float m_firingSpeed;
	float m_timeElapsed;
	int m_damage;
};
