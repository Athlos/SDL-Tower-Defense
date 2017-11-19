#pragma once

class AnimatedSprite;
class Sprite;
class BackBuffer;
class Texture;
class Entity;

#include "entity.h"

class Explosion : public Entity
{
	//Member Methods:
public:
	Explosion();
	~Explosion();

	bool Initialise(AnimatedSprite* sprite); // Initialise the explosion

	void Process(float deltaTime); // Process the animation
	void Draw(BackBuffer& backBuffer); // Draw the explosion frame

	void SetPosition(float x, float y); // Set the screen position

	// Member Data:
protected:
	AnimatedSprite* m_pSprite; // Anmiated sprite for explosion
	
};

