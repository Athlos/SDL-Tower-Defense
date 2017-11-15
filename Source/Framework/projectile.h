#pragma once

#include "entity.h"

class Projectile : public Entity
{
	//Member Methods:
public:
	Projectile(int damage, float speed, float lifespan);
	~Projectile();

	void Process(float deltaTime);

	void SetDamage(int damage);
	int GetDamage() const;

	void SetLifespan(float lifespan);
	float GetLifespan() const;

	void SetSpeed(float speed);
	float GetSpeed() const;

	void SetTarget(Position* pos);

	//Member Data:
private:
	int m_damage;
	float m_speed;
	float m_lifespan;

	float m_directionX;
	float m_directionY;
};

