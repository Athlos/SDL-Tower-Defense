#include "tower.h"
#include "tile.h"
#include "enemy.h"
#include "axisalignedboundingbox.h"
#include "backbuffer.h"

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

	m_currentTarget->TakeDamage(m_damage);

	if (m_currentTarget->IsDead())
	{
		EvaluateTarget();
	}
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
	m_targetsInRange.erase(m_targetsInRange.begin());
	/*int targetIndex = 0;

	float closest = sqrt((m_currentTarget->GetCenterX() - GetCenterX()) * (m_currentTarget->GetCenterX() - GetCenterX()) + (m_currentTarget->GetCenterY() - GetCenterY()) * (m_currentTarget->GetCenterY() - GetCenterY())) > m_tileRange * m_tilePosition->GetTileWidth();

	for (targetIndex = 1; targetIndex < m_targetsInRange.size(); ++targetIndex)
	{
		float currentDistance = sqrt((m_targetsInRange[targetIndex]->GetCenterX() - GetCenterX()) * (m_targetsInRange[targetIndex]->GetCenterX() - GetCenterX()) + (m_targetsInRange[targetIndex]->GetCenterY() - GetCenterY()) * (m_targetsInRange[targetIndex]->GetCenterY() - GetCenterY())) > m_tileRange * m_tilePosition->GetTileWidth();

		if (currentDistance < closest)
		{
			m_currentTarget = m_targetsInRange[targetIndex];
			closest = currentDistance;
		}
	}

	m_targetsInRange.erase(m_targetsInRange.begin() + targetIndex - 1);*/
}

int Tower::GetTowerCost()
{
	return m_cost;
}
