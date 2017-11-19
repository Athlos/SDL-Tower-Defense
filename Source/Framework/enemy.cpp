
#include "enemy.h";

#include "entity.h";
#include "sprite.h"
#include "tile.h"
#include "grid.h"
#include "backbuffer.h"
#include "position.h"
#include "animatedsprite.h"

#include <math.h>
#include <cassert>

Enemy::Enemy()
	: m_reachedEnd(false)
	, m_directionX(0.0f)
	, m_directionY(0.0f)
	, m_distanceToNextWaypoint(0.0f)
	, m_targetted(false)
	, m_inRange(false)
{
	Entity::Entity();
}

Enemy::~Enemy() 
{
	Entity::~Entity();
}

void Enemy::SetPosition(float x, float y)
{
	//Entity::SetPosition(x, y);

	m_animSprite->SetX(static_cast<int>(m_x));
	m_animSprite->SetY(static_cast<int>(m_y));

	m_pos->m_x = GetCenterX();
	m_pos->m_y = GetCenterY();
}

float Enemy::GetCenterX() const
{
	return static_cast<float>(m_animSprite->GetCenterX());
}

float Enemy::GetCenterY() const
{
	return static_cast<float>(m_animSprite->GetCenterY());
}

bool Enemy::Initialise(AnimatedSprite * sprite)
{
	assert(sprite);
	m_animSprite = sprite;

	m_animSprite->SetX(m_x);
	m_animSprite->SetY(m_y);

	return (true);
}

void Enemy::Process(float deltaTime)
{
	//Entity::Process(deltaTime);

	MoveToWaypoints(deltaTime);

	//m_x += deltaTime * m_velocityX;
	//m_y += deltaTime * m_velocityY;

	//if (m_x >= 800 - m_pSprite->GetWidth())
	//{
	//	m_x = (800 - m_pSprite->GetWidth());
	//	m_velocityX = 0;
	//}
	//if (m_x <= 0)
	//{
	//	m_x = 0;
	//	m_velocityX = 0;
	//}

	m_animSprite->Process(deltaTime);

	m_animSprite->SetX(static_cast<int>(m_x));
	m_animSprite->SetY(static_cast<int>(m_y));
}

void Enemy::Draw(BackBuffer& backBuffer)
{
	m_animSprite->Draw(backBuffer);

	DrawHealthBar(backBuffer);
}

void Enemy::DrawHealthBar(BackBuffer& backBuffer)
{
	//Draw Frame
	backBuffer.SetDrawColour(0, 0, 0);
	backBuffer.DrawRectangle(m_x, m_y - 4, m_x + 32, m_y, 0);

	//Draw Fill
	backBuffer.SetDrawColour(255, 0, 0);
	backBuffer.DrawRectangle(m_x, m_y - 4, m_x + (((float)m_currentHealth / (float)m_maxHealth) * 32), m_y, 1);
}

void Enemy::SetData(int health, float speed, int damage)
{
	m_maxHealth = health;

	m_currentHealth = m_maxHealth;

	m_speed = speed;
	m_damage = damage;

	//Calculate reward
	m_reward = 0;
	m_reward += m_currentHealth * 5;
	m_reward += m_speed * .002;
}

bool Enemy::IsClickedOn(int x, int y)
{
	return (Entity::IsClickedOn(x, y));
}

int Enemy::GetDamage()
{
	return m_damage;
}

void Enemy::TakeDamage(int amount)
{
	m_currentHealth -= amount;

	if (m_currentHealth <= 0)
	{
		m_currentHealth = 0;
		m_dead = true;
	}
}

int Enemy::GetReward()
{
	return m_reward;
}

void Enemy::SetReward(int amount) 
{
	m_reward = amount;
}

void Enemy::SetTilePosition(Tile* tile)
{
	m_tilePosition = tile;

	SetPosition(tile->m_xScreenPos, tile->m_yScreenPos);
}

Tile* Enemy::GetTilePosition()
{
	return m_tilePosition;
}

void Enemy::SetPath(std::queue<Position*> path)
{
	m_waypoints = path;

	if (!path.empty())
	{
		m_currentWaypoint = m_waypoints.front();
		m_waypoints.pop();
	}

	UpdateDirection();
}

void Enemy::MoveToWaypoints(float deltaTime)
{
	if (m_currentWaypoint == 0)
	{
		return;
	}

	//assert(m_x >= 0);
	//assert(m_y >= 0);

	float xDiff = abs(m_pos->m_x - m_currentWaypoint->m_x);
	float yDiff = abs(m_pos->m_y - m_currentWaypoint->m_y);

	if (xDiff < 2 && yDiff < 2)
	{
		if (m_waypoints.empty())
		{
			m_reachedEnd = true;
			m_currentWaypoint = 0;

			return;
		}
		else
		{
			m_currentWaypoint = m_waypoints.front();
			m_waypoints.pop();

			UpdateDirection();
		}
	}

	m_x += deltaTime * m_directionX * m_speed;
	m_y += deltaTime * m_directionY * m_speed;

	SetPosition(m_x, m_y);

	m_distanceToNextWaypoint = sqrt((m_currentWaypoint->m_x - m_pos->m_x) * (m_currentWaypoint->m_x - m_pos->m_x) + (m_currentWaypoint->m_y - m_pos->m_y) * (m_currentWaypoint->m_y - m_pos->m_y));
}

bool Enemy::ReachedEnd() const
{
	return m_reachedEnd;
}

int Enemy::WaypointsToGo() const
{
	return m_waypoints.size();
}

float Enemy::DistanceToNextWaypoint() const
{
	return m_distanceToNextWaypoint;
}

void Enemy::UpdateDirection()
{
	if (m_currentWaypoint != 0)
	{
		float angle = atan2(GetCenterX() - m_currentWaypoint->m_x, GetCenterY() - m_currentWaypoint->m_y);

		angle *= 180 / 3.1459f;
		angle *= -1;

		angle += 180;

		m_animSprite->SetAngle(angle);

		//m_animSprite->SetAngle(angle);

		m_directionX = m_currentWaypoint->m_x - GetCenterX();
		m_directionY = m_currentWaypoint->m_y - GetCenterY();

		float hyp = sqrt(m_directionX*m_directionX + m_directionY*m_directionY);

		m_directionX /= hyp;
		m_directionY /= hyp;
	}
}