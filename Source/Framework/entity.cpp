
// This include:
#include "entity.h"

// Local includes:
#include "game.h"
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "position.h"
#include "axisalignedboundingbox.h"
#include "sprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>
#include <SDL.h>

Entity::Entity()
: m_pSprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
{
	m_pos = new Position(m_x, m_y);
	m_bounds = new AxisAlignedBoundingBox(m_pos, 16);
}

Entity::~Entity()
{
	delete m_pSprite;
	m_pSprite = 0;

	delete m_bounds;
	m_bounds = 0;
}

bool Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	m_pSprite->SetX(m_x);
	m_pSprite->SetY(m_y);

	return (true);
}

void Entity::Process(float deltaTime)
{
	m_x += deltaTime * m_velocityX;
	m_y += deltaTime * m_velocityY;

	if (m_x >= 800 - m_pSprite->GetWidth())
	{
		m_x = (800 - m_pSprite->GetWidth());
		m_velocityX = 0;
	}
	if (m_x <= 0)
	{
		m_x = 0;
		m_velocityX = 0;
	}

	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
}

void Entity::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->Draw(backBuffer);

	//backBuffer.DrawRectangle(m_bounds->center->m_x - m_bounds->halfDimension, m_bounds->center->m_y + m_bounds->halfDimension, m_bounds->center->m_x + m_bounds->halfDimension, m_bounds->center->m_y - m_bounds->halfDimension, 0);
}

void Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool Entity::IsDead() const
{
	return m_dead;
}

bool Entity::IsCollidingWith(int x, int y, int radius)
{
	if (sqrt((x - m_x) * (x - m_x) + (y - m_y) * (y - m_y)) < radius + 16)
	{
		return true;
	}
	return (false);
}

bool Entity::IsClickedOn(int x, int y)
{
	int x1 = x + 8;
	int y1 = y + 8;
	int radius1 = 16;
	int radius2 = 16;

	int x2 = m_x + 16;
	int y2 = m_y + 16;

	//compare the distance to combined radii
	if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < 32)
	{
		return true;
	}
	return (false);
}

float Entity::GetPositionX() const
{
	return (m_x);
}

float Entity::GetPositionY() const
{
	return (m_y);
}

float Entity::GetCenterX() const
{
	return static_cast<float>(m_pSprite->GetCenterX());
}

float Entity::GetCenterY() const
{
	return static_cast<float>(m_pSprite->GetCenterY());
}

float Entity::GetHorizontalVelocity()
{
	return (m_velocityX);
}

void Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

float Entity::GetVerticalVelocity()
{
	return (m_velocityY);
}

void Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}

Sprite* Entity::GetSprite()
{
	return m_pSprite;
}

Position* Entity::GetPosition() const
{
	return m_pos;
}

void Entity::SetPosition(float x, float y) 
{
	m_x = x;
	m_y = y;
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));

	m_pos->m_x = GetCenterX();
	m_pos->m_y = GetCenterY();
}

AxisAlignedBoundingBox* Entity::GetCollisionBounds() const
{
	return m_bounds;
}

void Entity::SetCollisionBounds(Position * center, float radius)
{
	m_bounds = new AxisAlignedBoundingBox(center, radius);
}
