#include "projectile.h"
#include "position.h"
#include "sprite.h"

#include <math.h>

Projectile::Projectile(int damage, float speed, float lifespan)
{
	m_damage = damage;
	m_speed = speed;
	m_lifespan = lifespan;
}

Projectile::~Projectile()
{

}

void Projectile::Process(float deltaTime)
{
	//Entity::Process(deltaTime);

	m_x += deltaTime * m_directionX * m_speed;
	m_y += deltaTime * m_directionY * m_speed;

	SetPosition(m_x, m_y);
}

void Projectile::SetDamage(int damage)
{
	m_damage = damage;
}

int Projectile::GetDamage() const
{
	return m_damage;
}

void Projectile::SetLifespan(float lifespan)
{
	m_lifespan = lifespan;
}

float Projectile::GetLifespan() const
{
	return m_lifespan;
}

void Projectile::SetSpeed(float speed)
{
	m_speed = speed;
}

float Projectile::GetSpeed() const
{
	return m_speed;
}

void Projectile::SetTarget(Position* pos)
{
	float angle = atan2(GetCenterX() - pos->m_x, GetCenterY() - pos->m_y);

	angle *= 180 / 3.1459f;
	angle *= -1;

	m_pSprite->SetAngle(angle);

	m_directionX = pos->m_x - GetCenterX();
	m_directionY = pos->m_y - GetCenterY();

	float hyp = sqrt(m_directionX*m_directionX + m_directionY*m_directionY);

	m_directionX /= hyp;
	m_directionY /= hyp;
}