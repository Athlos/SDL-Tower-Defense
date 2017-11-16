#include "tower.h"
#include "tile.h"
#include "enemy.h"
#include "axisalignedboundingbox.h"
#include "backbuffer.h"
#include "projectile.h"
#include "game.h"
#include "sprite.h"

#include <cassert>

Tower::Tower(int range, float firingSpeed, int damage, int cost)
{
	Entity::Entity();
	m_tileRange = range;
	m_firingSpeed = firingSpeed;
	m_damage = damage;
	m_cost = cost;

	m_currentTarget = 0;
	m_timeElapsed = 0;
}

Tower::~Tower()
{
	Entity::Entity();
}

bool Tower::Initialise(BackBuffer* backBuffer)
{
	m_pSprite = backBuffer->CreateSprite("assets\\tower_base.png");

	assert(m_pSprite);

	m_pSprite->SetX(m_x);
	m_pSprite->SetY(m_y);

	m_backBuffer = backBuffer;

	return (true);
}

void Tower::Process(float deltaTime)
{
	Entity::Process(deltaTime);

	m_timeElapsed += deltaTime;

	if (m_currentTarget != 0)
	{
		if (m_currentTarget->IsDead())
		{
			EvaluateTarget();
		}

		if (m_timeElapsed >= m_firingSpeed && m_currentTarget != 0)
		{
			Shoot();
		}
	}
}

void Tower::Draw(BackBuffer& backBuffer)
{
	backBuffer.SetDrawColour(0, 0, 0);
	Entity::Draw(backBuffer);

	//backBuffer.SetDrawColour(0, 125, 0, 1);
	//backBuffer.DrawRectangle(m_towerRangeArea->center->m_x - m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_y + m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_x + m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_y - m_towerRangeArea->halfDimension, 0);
}

void Tower::Shoot()
{
	m_timeElapsed = 0;

	Projectile* firedProjectile = new Projectile(m_damage, 500, 1.0f);

	Sprite* projectileSprite = m_backBuffer->CreateSprite("assets\\projectile.png");

	firedProjectile->Initialise(projectileSprite);

	firedProjectile->SetPosition(m_pos->m_x, m_pos->m_y);
	firedProjectile->SetTarget(m_currentTarget->GetPosition());

	Game::GetInstance().AddProjectile(firedProjectile);
}

void Tower::SetTilePosition(Tile* tile)
{
	Entity::SetPosition(tile->GetX(), tile->GetY());
	m_tilePosition = tile;

	m_towerRangeArea = new AxisAlignedBoundingBox(m_pos, m_tileRange * tile->GetTileWidth() + (tile->GetTileWidth() / 2));
}

void Tower::SetEnemiesInRange(std::vector<Entity*> enemies)
{
	//if (m_currentTarget != 0 )
	//{
	//	if (m_currentTarget->GetCollisionBounds()->intersects(m_towerRangeArea)) // if current target is in range, do not refresh enemies
	//	{
	//		return;
	//	}
	//}

	m_targetsInRange.clear();

	for each (Entity* e in enemies)
	{
		if (!e->IsDead())
		{
			m_targetsInRange.push_back(reinterpret_cast<Enemy*>(e));
		}
	}

	EvaluateTarget();
}

AxisAlignedBoundingBox* Tower::GetRangeBounds()
{
	return m_towerRangeArea;
}

void Tower::SetCurrentTarget(Enemy* target)
{
	m_currentTarget = target;
}

void Tower::EvaluateTarget()
{
	m_currentTarget = 0;

	if (m_targetsInRange.empty())
	{
		return;
	}

	m_currentTarget = m_targetsInRange[0];

	int eraseTargetCounter = 0;

	for (int i = 1; i < m_targetsInRange.size(); ++i) // Evaluate all remaining targets
	{
		if (m_targetsInRange[i]->WaypointsToGo() <= m_currentTarget->WaypointsToGo()) // Has target gone through more or equal amount of waypoints through the map
		{
			if (m_targetsInRange[i]->WaypointsToGo() == m_currentTarget->WaypointsToGo()) // If they are both going to the same waypoint, check which is closer
			{
				if (m_targetsInRange[i]->DistanceToNextWaypoint() < m_currentTarget->DistanceToNextWaypoint()) // Check which is closer to the matching waypoint
				{
					m_currentTarget = m_targetsInRange[i];
					eraseTargetCounter = i;
				}
			}
			else // New enemy is at a farther waypoint, its now the closest target
			{
				m_currentTarget = m_targetsInRange[i];
				eraseTargetCounter = i;
			}
		}
	}

	m_targetsInRange.erase(m_targetsInRange.begin() + eraseTargetCounter);
}

int Tower::GetTowerCost()
{
	return m_cost;
}
