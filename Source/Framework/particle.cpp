#include "particle.h"
#include "entity.h"
#include "texture.h"
#include "sprite.h"

Particle::Particle()
{

}

Particle::~Particle() 
{

}

bool Particle::Initialise(Sprite* sprite)
{
	return (Entity::Initialise(sprite));
}

void Particle::Process(float deltaTime)
{
	m_velocityX += m_accelerationX * deltaTime;
	m_velocityY += m_accelerationY * deltaTime;

	if (m_velocityX <= 1 && m_velocityX >= -1)
	{
		m_accelerationX = 0;
	}

	if (m_velocityY <= 1 && m_velocityY >= -1) 
	{
		m_accelerationY = 0;
	}
	//If particle is of type text, it is essentially a moving label with a lifespan, so it must be moved separatly
	Entity::Process(deltaTime);
	m_age -= deltaTime;

	//set death immediatly if out of screen bounds
	if (m_y >= 595) 
	{
		m_age = 0;
	}
}

void Particle::Draw(BackBuffer& backbuffer)
{
	Entity::Draw(backbuffer);
}

void Particle::SetAccelerationX(float x)
{
	m_accelerationX = x;
}

void Particle::SetAccelerationY(float y)
{
	m_accelerationY = y;
}

float Particle::GetAge()
{
	return m_age;
}

void Particle::SetAge(float age)
{
	m_age = age;
}

void Particle::SpawnParticle(int x, int y, BackBuffer* backbuffer)
{
	return;
}

ParticleType Particle::GetType()
{
	return m_type;
}