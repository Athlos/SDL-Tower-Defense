#include "playerbullettrailparticle.h"
#include "sprite.h"
#include "entity.h"
#include "logmanager.h"


#include <vector>
#include <cmath>
#include <ctime>



PlayerBulletTrailParticle::PlayerBulletTrailParticle()
{
	Particle::Particle();
	m_type = PLAYERBULLETTRAIL;
}

PlayerBulletTrailParticle::~PlayerBulletTrailParticle() 
{

}

bool PlayerBulletTrailParticle::Initialise(Sprite* sprite)
{
	return (Particle::Initialise(sprite));
}

void PlayerBulletTrailParticle::Process(float deltaTime)
{
	Particle::Process(deltaTime);
	
}
void PlayerBulletTrailParticle::Draw(BackBuffer& backbuffer)
{

	Particle::Draw(backbuffer);
}


void PlayerBulletTrailParticle::SpawnParticle(int x, int y, BackBuffer* backbuffer, float duration, ParticleType t)
{
	m_pSprite = backbuffer->CreateSprite("assets\\playerbullettrail.png");
	
	m_x = x;
	m_y = y;
	m_age = duration;
	m_velocityX = (rand() % 100) - 50;
	m_velocityY = (rand() % 100) - 50;
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	if (static_cast<int>(m_x) <= 0) 
	{
		LogManager::GetInstance().Log("particle at zero");
	}
	//OBJECTS MUST DE-ACCELERATE, MAKE ACCELERATION OPOSITE TYPE TO VELOCITY
	if (m_velocityX > 0) 
	{
		m_accelerationX = (rand() % 50) - 50;
	}
	else 
	{
		m_accelerationX = rand() % 50;
	}

	if (m_velocityY > 0) 
	{
		m_accelerationY = (rand() % 50) - 50;
	}
	else
	{
		m_accelerationY = rand() % 50;
	}
}