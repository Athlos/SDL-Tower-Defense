#include "pulsetower.h"
#include "backbuffer.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "enemy.h"
#include "tile.h"
#include "game.h"

#include <cassert>

PulseTower::PulseTower() : Tower(PULSE)
{
	m_tileRange = 1;
	m_firingSpeed = 1.5f;
	m_damage = 2;
}

PulseTower::~PulseTower()
{
}

bool PulseTower::Initialise(BackBuffer * backBuffer)
{
	Tower::Initialise(backBuffer);

	m_towerSprite = backBuffer->CreateAnimatedSprite("assets\\tower_pulse.png");
	m_attackSprite = backBuffer->CreateAnimatedSprite("assets\\attack_pulse.png");

	m_towerSprite->LoadFrames(32, 32);
	m_attackSprite->LoadFrames(32, 32);

	m_towerSprite->SetX(m_x);
	m_towerSprite->SetY(m_y);

	m_towerSprite->SetFrameSpeed((m_firingSpeed / m_towerSprite->GetFrameCount()) / 2);
	m_towerSprite->SetLooping(false);

	m_attackSprite->SetFrameSpeed((m_firingSpeed / m_attackSprite->GetFrameCount()) / 2);
	m_attackSprite->SetLooping(false);
	m_attackSprite->StopAnimating();

	return true;
}

void PulseTower::Process(float deltaTime)
{
	Tower::Process(deltaTime);

	m_attackSprite->Process(deltaTime);

	m_attackSprite->SetX(m_x - (m_tilePosition->GetTileWidth() * m_tileRange));
	m_attackSprite->SetY(m_y - (m_tilePosition->GetTileHeight() * m_tileRange));

	if (!m_targetsInRange.empty())
	{
		if (m_timeElapsed >= m_firingSpeed)
		{
			Shoot();
			m_towerSprite->StartAnimating();
			m_attackSprite->StartAnimating();
		}
	}
}

void PulseTower::Draw(BackBuffer & backBuffer)
{
	Tower::Draw(backBuffer);

	if (m_attackSprite->IsAnimating())
	{
		m_attackSprite->Draw(backBuffer);
	}
}

void PulseTower::Shoot()
{
	m_timeElapsed = 0;

	for each (Enemy* enemy in m_targetsInRange)
	{
		enemy->TakeDamage(m_damage);
	}

	Game::GetInstance().PlaySound("assets\\audio\\pulse_shoot.wav");
}

void PulseTower::SetScale()
{
	Tower::SetScale();

	int xPos = m_x - (m_tilePosition->GetTileWidth() * m_tileRange);
	int yPos = m_y - (m_tilePosition->GetTileHeight() * m_tileRange);

	m_attackSprite->SetX(xPos);
	m_attackSprite->SetY(yPos);

	int scale = m_tileRange * 2 + 1;

	m_attackSprite->SetScale(m_tilePosition->GetTileWidth() * scale, m_tilePosition->GetTileHeight() * scale);
}
