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

Tower::Tower(TowerType type) : Building(0)
	, m_timeElapsed(0)
	, m_maxLevel(4)
	, m_currentLevel(1)
{
	m_buildingType = TOWER;
	m_towerType = type;
}

Tower::~Tower()
{
	Entity::Entity();
}

bool Tower::Initialise(BackBuffer* backBuffer)
{
	m_pSprite = backBuffer->CreateSprite("assets\\tower_base.png");
	m_selectionOutline = backBuffer->CreateSprite("assets\\tower_selected.png");

	assert(m_pSprite);
	assert(m_selectionOutline);

	m_pSprite->SetX(m_x);
	m_pSprite->SetY(m_y);

	m_selectionOutline->SetX(m_x);
	m_selectionOutline->SetY(m_y);

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
}

void Tower::SetTilePosition(Tile* tile)
{
	Building::SetTilePosition(tile);

	m_towerRangeArea = new AxisAlignedBoundingBox(m_pos, m_tileRange * tile->GetTileWidth() + (tile->GetTileWidth() / 2));

	//Scale to tile size
	SetScale();
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
}

AxisAlignedBoundingBox* Tower::GetRangeBounds()
{
	return m_towerRangeArea;
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

TowerType Tower::GetTowerType()
{
	return m_towerType;
}

void Tower::SetScale()
{
	int tileWidth = m_tilePosition->GetTileWidth();
	int tileHeight = m_tilePosition->GetTileHeight();

	m_towerSprite->SetX(m_x);
	m_towerSprite->SetY(m_y);

	m_towerSprite->SetScale(tileWidth, tileHeight);
}
