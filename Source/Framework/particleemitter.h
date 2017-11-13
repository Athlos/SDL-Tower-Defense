#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__

#include "particle.h"
#include "entity.h"
#include "playerbullettrailparticle.h"
#include <vector>

class ParticleEmitter : public Entity
{
	// Member Methods:
public:
	ParticleEmitter();
	~ParticleEmitter();

	void SpawnNewParticles(int x, int y, int amount, BackBuffer* backbuffer, ParticleType T);
	void SpawnTextParticle(int x, int y, std::string message);
	bool Initialise(Sprite* sprite);

	void SetParticleTimer(float timer);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	// Member Data:
protected:
	std::vector<Particle*> m_particles;
	
	float m_particleTimer; // how often particles spawn

};
#endif //__PARTICLEEMITTER_H__