#pragma once

#include "entity.h"

class Projectile : public Entity
{
	//Member Methods:
public:
	Projectile(int damage, float speed, float lifespan);
	~Projectile();

	void Process(float deltaTime); // Process the projectile

	void SetDamage(int damage); // Set the damage the projectile can do
	int GetDamage() const; // Get the damage the projectile can do

	void SetLifespan(float lifespan); // Set the lifespan of the bullet, not used atm
	float GetLifespan() const; // Get lifespan of the bullet, not used atm

	void SetSpeed(float speed); // Set the speed of the bullet
	float GetSpeed() const; // Get the speed of the bullet

	void SetTarget(Position* pos); // Set the position the bullet is being fired towards

	//Member Data:
private:
	int m_damage; // Damage of the bullet
	float m_speed; // Speed of the bullet
	float m_lifespan; // Duration the bullet is active

	float m_directionX; // X direction of the bullet
	float m_directionY; // Y direction of the bullet
};

