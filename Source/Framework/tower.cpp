#include "tower.h"
#include "tile.h"
#include "enemy.h"
#include "axisalignedboundingbox.h"
#include "backbuffer.h"
#include "projectile.h"
#include "game.h"
#include "sprite.h"
#include "animatedsprite.h"

#include <cassert>
#include <sstream>

Tower::Tower(int range, float firingSpeed, int damage, int cost) : Building(cost)
{
	m_tileRange = range;
	m_firingSpeed = firingSpeed;
	m_damage = damage;

	m_currentTarget = 0;
	m_timeElapsed = 0;
	m_currentLevel = 1;
	m_maxLevel = 4;

	m_buildingType = TOWER;
}

Tower::~Tower()
{
	Entity::Entity();
}

bool Tower::Initialise(BackBuffer* backBuffer)
{
	m_pSprite = backBuffer->CreateSprite("assets\\tower_base.png");
	m_selectionOutline = backBuffer->CreateSprite("assets\\tower_selected.png");

	m_towerSprite = backBuffer->CreateAnimatedSprite("assets\\tower_loading.png");
	m_towerSprite->LoadFrames(32, 32);
	//m_towerSprite->SetFrameSpeed(0.1f);

	assert(m_pSprite);

	m_pSprite->SetX(m_x);
	m_pSprite->SetY(m_y);

	m_towerSprite->SetX(m_x);
	m_towerSprite->SetY(m_y);

	m_selectionOutline->SetX(m_x);
	m_selectionOutline->SetY(m_y);

	m_towerSprite->SetFrameSpeed(m_firingSpeed / m_towerSprite->GetFrameCount());
	m_towerSprite->SetLooping(false);

	m_backBuffer = backBuffer;

	return (true);
}

void Tower::Process(float deltaTime)
{
	Building::Process(deltaTime);

	m_towerSprite->SetX(m_x);
	m_towerSprite->SetY(m_y);

	m_timeElapsed += deltaTime;

	m_towerSprite->Process(deltaTime);

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

void Tower::Draw(BackBuffer& backBuffer)
{
	Building::Draw(backBuffer);

	m_towerSprite->Draw(backBuffer);
	
	//backBuffer.SetDrawColour(0, 125, 0, 1);
	//backBuffer.DrawRectangle(m_towerRangeArea->center->m_x - m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_y + m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_x + m_towerRangeArea->halfDimension, m_towerRangeArea->center->m_y - m_towerRangeArea->halfDimension, 0);
}

void Tower::Shoot()
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

void Tower::SetTilePosition(Tile* tile)
{
	Building::SetTilePosition(tile);

	m_towerRangeArea = new AxisAlignedBoundingBox(m_pos, m_tileRange * tile->GetTileWidth() + (tile->GetTileWidth() / 2));

	//Scale to tile size
	int tileWidth = m_tilePosition->GetTileWidth();
	int tileHeight = m_tilePosition->GetTileHeight();

	m_towerSprite->SetX(m_x);
	m_towerSprite->SetY(m_y);

	m_towerSprite->SetScale(tileWidth, tileHeight);
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

int Tower::GetTowerRange() const
{
	return m_tileRange;
}

int Tower::GetTowerLevel() const
{
	return m_currentLevel;
}

int Tower::GetTowerDamage() const
{
	return m_damage;
}

float Tower::GetTowerFireRate() const
{
	return m_firingSpeed;
}

int Tower::GetSellValue() const
{
	int value = 0;

	value += m_damage * 15;
	value += (1.0f / m_firingSpeed) * 15;
	value += m_tileRange * 10;

	value *= m_currentLevel;

	return value;
}

int Tower::GetTowerUpgradeCost() const
{
	int upgradeCost = 150;

	upgradeCost *= (m_currentLevel * m_currentLevel);

	return upgradeCost;
}

void Tower::UpgradeTower()
{
	if (m_currentLevel < m_maxLevel)
	{
		m_damage *= 2.5f;

		m_firingSpeed *= 0.75f;

		m_tileRange += 0;

		++m_currentLevel;

		m_towerSprite->SetFrameSpeed(m_firingSpeed / m_towerSprite->GetFrameCount());

		delete m_pSprite;
		m_pSprite = 0;

		std::stringstream levelSprite;
		levelSprite << "assets\\tower_base_level" << m_currentLevel << ".png";

		m_pSprite = m_backBuffer->CreateSprite(levelSprite.str().c_str());

		int tileWidth = m_tilePosition->GetTileWidth();
		int tileHeight = m_tilePosition->GetTileHeight();

		m_pSprite->SetX(m_x);
		m_pSprite->SetY(m_y);

		m_pSprite->SetWidth(tileWidth);
		m_pSprite->SetHeight(tileHeight);
	}
}

bool Tower::IsMaxLevel()
{
	return m_currentLevel == m_maxLevel;
}
