#include "explosion.h"
#include "backbuffer.h"
#include "animatedsprite.h"
#include <cassert>

Explosion::Explosion()
{
	Entity::Entity();
}

Explosion::~Explosion()
{

}

bool Explosion::Initialise(AnimatedSprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;
	m_pSprite->SetFrameWidth(64);
	//m_pSprite->AddFrame(0);
	//m_pSprite->AddFrame(64);
	//m_pSprite->AddFrame(128);
	//m_pSprite->AddFrame(192);
	//m_pSprite->AddFrame(256);
	return (true);
}

void Explosion::Process(float deltaTime)
{
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
	m_pSprite->SetFrameSpeed(0.08f);
	m_pSprite->Process(deltaTime);
	if (!m_pSprite->IsAnimating()) 
	{
		m_dead = true;
	}
}

void Explosion::Draw(BackBuffer& backBuffer)
{
	assert(m_pSprite);
	m_pSprite->Draw(backBuffer);
}

void Explosion::SetPosition(float x, float y) 
{
	m_x = x;
	m_y = y;
	m_pSprite->SetX(static_cast<int>(m_x));
	m_pSprite->SetY(static_cast<int>(m_y));
}
