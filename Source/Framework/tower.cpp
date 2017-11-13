#include "tower.h"
#include "tile.h"
#include "enemy.h"

Tower::Tower(int range, float firingSpeed, int damage)
{
	m_tileRange = range;
	m_firingSpeed = firingSpeed;
	m_damage = damage;

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

	if (m_timeElapsed >= m_firingSpeed && HasValidTarget())
	{
		Shoot();
	}
}

void Tower::Draw(BackBuffer& backBuffer)
{
	Entity::Draw(backBuffer);
}

void Tower::Shoot()
{
	m_timeElapsed = 0;

	m_currentTarget->m_enemyTarget->TakeDamage(m_damage);

	if (m_currentTarget->m_enemyTarget->IsDead())
	{
		m_currentTarget = 0;
	}
}

void Tower::SetTilePosition(Tile* tile)
{
	Entity::SetPosition(tile->GetX(), tile->GetY());
	m_tilePosition = tile;
}

void Tower::GetEnemiesInRange(std::vector<Enemy*> enemies)
{
	if (enemies.empty())
	{
		return;
	}

	SetCurrentTarget(0);
	float closestDistance = sqrt((enemies.at(0)->GetCenterX() - GetCenterX()) * (enemies.at(0)->GetCenterX() - GetCenterX()) + (enemies.at(0)->GetCenterY() - GetCenterY()) * (enemies.at(0)->GetCenterY() - GetCenterY()));

	if (enemies.size() == 1)
	{
		SetCurrentTarget(new Target(enemies.at(0), closestDistance)); // Update target to shoot
	}
	else // there are more than 1 enemies to compare
	{
		float rangeInPixels = m_tileRange * m_tilePosition->GetTileWidth(); // convert tile range to pixels

		for each (Enemy* enemy in enemies) // check all enemies
		{
			float distance = sqrt((enemy->GetCenterX() - GetCenterX()) * (enemy->GetCenterX() - GetCenterX()) + (enemy->GetCenterY() - GetCenterY()) * (enemy->GetCenterY() - GetCenterY())); // get distance between center of enemy and tower

			if (distance < rangeInPixels) // check distance between centers is within tower range
			{
				SetCurrentTarget(new Target(enemy, distance)); // Update target to shoot
				break;
			}
		}
	}
}

void Tower::SetCurrentTarget(Target* target)
{

	m_currentTarget = target;
}

bool Tower::HasValidTarget()
{
	if (m_currentTarget == 0)
	{
		return false;
	}
	else if (sqrt((m_currentTarget->m_enemyTarget->GetCenterX() - GetCenterX()) * (m_currentTarget->m_enemyTarget->GetCenterX() - GetCenterX()) + (m_currentTarget->m_enemyTarget->GetCenterY() - GetCenterY()) * (m_currentTarget->m_enemyTarget->GetCenterY() - GetCenterY())) > m_tileRange * m_tilePosition->GetTileWidth()) // check distance is in range
	{
		return true;
	}
	else
	{
		m_currentTarget = 0;
		return false;
	}
}