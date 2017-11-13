#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "entity.h"

#include <cmath>
#include <ctime>

enum ParticleType
{
	PLAYERBULLETTRAIL,
	TEXT
};

class Particle : public Entity
{
	// Member Methods:
public:
	Particle();
	~Particle();

	bool Initialise(Sprite* sprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetAccelerationX(float x);
	void SetAccelerationY(float y);

	float GetAge();
	void SetAge(float age);

	void SpawnParticle(int x, int y, BackBuffer* backbuffer);
	ParticleType GetType();

	// Member Data:
protected:
	float m_accelerationX;
	float m_accelerationY;

	float m_age;
	ParticleType m_type;

};
#endif //__PARTICLE_H__

