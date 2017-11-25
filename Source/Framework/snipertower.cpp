#include "snipertower.h"
#include "enemy.h"
#include "projectile.h"
#include "game.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "backbuffer.h"
#include "position.h"

SniperTower::SniperTower() : Tower(SNIPER)
	, m_currentTarget(0)
{
	m_tileRange = 2;
	m_firingSpeed = 0.5f;
	m_damage = 1;
}

SniperTower::~SniperTower()
{
}

bool SniperTower::Initialise(BackBuffer* backBuffer)
{
	Tower::Initialise(backBuffer);

	m_towerSprite = backBuffer->CreateAnimatedSprite("assets\\tower_sniper.png");

	m_towerSprite->LoadFrames(32, 32);

	m_towerSprite->SetX(m_x);
	m_towerSprite->SetY(m_y);

	m_towerSprite->SetFrameSpeed(m_firingSpeed / m_towerSprite->GetFrameCount());
	m_towerSprite->SetLooping(false);

	return (true);
}

void SniperTower::Process(float deltaTime)
{
	Tower::Process(deltaTime);

	if (m_currentTarget != 0)
	{
		if (m_currentTarget->IsDead())
		{
			EvaluateTarget();
		}

		if (m_timeElapsed >= m_firingSpeed && m_currentTarget != 0)
		{
			Shoot();
			m_towerSprite->StartAnimating();
		}

		float angle = atan2(GetCenterX() - m_currentTarget->GetPosition()->m_x, GetCenterY() - m_currentTarget->GetPosition()->m_y);

		angle *= 180 / 3.1459f;
		angle *= -1;
		m_towerSprite->SetAngle(angle);
	}
}

void SniperTower::Draw(BackBuffer& backBuffer)
{
	Building::Draw(backBuffer);

	m_towerSprite->Draw(backBuffer);

	//backBuffer.SetDrawColour(0, 125, 0, 1);
	//backBuffer.DrawRectangle(m_towerRangeArea->center->m_x - m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_y + m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_x + m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_y - m_towerRangeArea->halfDimension, 0);
}

void SniperTower::SetCurrentTarget(Enemy* target)
{
	m_currentTarget = target;
}

void SniperTower::EvaluateTarget()
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

void SniperTower::SetEnemiesInRange(std::vector<Entity*> enemies)
{
	Tower::SetEnemiesInRange(enemies);

	EvaluateTarget();
}

void SniperTower::Shoot()
{
	m_timeElapsed = 0;

	Projectile* firedProjectile = new Projectile(m_damage, 700, 1.0f);

	Sprite* projectileSprite = m_backBuffer->CreateSprite("assets\\projectile.png");
	projectileSprite->SetWidth(8);
	projectileSprite->SetHeight(8);

	firedProjectile->Initialise(projectileSprite);

	firedProjectile->SetPosition(m_pos->m_x, m_pos->m_y);
	firedProjectile->SetTarget(m_currentTarget->GetPosition());

	Game::GetInstance().AddProjectile(firedProjectile);

}