#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

#include <vector>
#include <queue>

class Tile;
class Grid;
class Position;

class Enemy : public Entity
{
	// Member Methods:
public:
	Enemy();
	~Enemy();

	void SetPosition(float x, float y);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void DrawHealthBar(BackBuffer& backBuffer);

	void SetData(int health, float speed, int damage);
	bool IsClickedOn(int x, int y);

	void TakeDamage(int amount);
	int GetDamage();

	int GetReward();
	void SetReward(int amount);

	void SetTilePosition(Tile* tile);
	Tile* GetTilePosition();

	void SetPath(std::queue<Position*> path);

	void MoveToWaypoints(float deltaTime);

	bool ReachedEnd() const;

private:
	void UpdateDirection();

	// Member Data:
public:
	Grid* m_grid;

	bool m_inRange;
	bool m_targetted;

protected:
	int m_currentHealth;
	int m_maxHealth;

	int m_damage;
	int m_reward;

	float m_speed;
	float m_directionX;
	float m_directionY;

	Tile* m_tilePosition;

	std::queue<Position*> m_waypoints;
	Position* m_currentWaypoint;

	bool m_reachedEnd;

};
#endif //__ENEMY_H__

