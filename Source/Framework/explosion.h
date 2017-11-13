#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

class AnimatedSprite;
class Sprite;
class BackBuffer;
class Texture;
class Entity;

#include "entity.h"


class Explosion : public Entity
{
	//Methods
public:
	Explosion();
	~Explosion();

	void SetPosition(float x, float y);
	void Draw(BackBuffer& backBuffer);
	bool Initialise(AnimatedSprite* sprite);
	void Process(float deltaTime);

	// Member Data:
protected:
	AnimatedSprite* m_pSprite;
	
};
#endif //__EXPLOSION_H__

