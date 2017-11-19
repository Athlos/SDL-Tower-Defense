#include "textparticle.h"
#include "label.h"

TextParticle::TextParticle()
{
	m_type = TEXT;
	m_text = new Label("");
	m_text->SetColour(255,0,0,0);
}


TextParticle::~TextParticle()
{
}

void TextParticle::Process(float deltaTime)
{
	//calculate velocity and position
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

	//Set position of base entity
	m_x += deltaTime * m_velocityX;
	m_y += deltaTime * m_velocityY;
	
	//Sync up label position to entity position
	m_text->SetBounds(m_x, m_y, 32, 24);

	m_age -= deltaTime;

	//set death immediatly if out of screen bounds
	if (m_y >= 595)
	{
		m_age = 0;
	}
}

void TextParticle::Draw(BackBuffer& backbuffer)
{
	m_text->Draw(backbuffer);
}

void TextParticle::SpawnParticle(int x, int y, std::string message)
{
	m_text->SetBounds(x, y, 32, 24);
	m_text->SetText(message);

	m_x = x;
	m_y = y;
	m_age = 0.5f;
	m_velocityX = (rand() % 150) - 75;
	m_velocityY = (rand() % 150) - 75;

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
