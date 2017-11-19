#pragma once

#include "particle.h"
#include "entity.h"
#include <vector>

class ParticleEmitter : public Entity
{
	// Member Methods:
public:
	ParticleEmitter();
	~ParticleEmitter();

	void Process(float deltaTime); // Process the particles
	void Draw(BackBuffer& backBuffer); // Draw the particles

	void SpawnNewParticles(int x, int y, int amount, BackBuffer* backbuffer, ParticleType T); // Spawn a new particle
	void SpawnTextParticle(int x, int y, std::string message); // Spawn a new text particle
	//bool Initialise(Sprite* sprite);

	void SetParticleTimer(float timer); // Set the time between spawning particles

	// Member Data:
protected:
	std::vector<Particle*> m_particles; // Container of all particles
	
	float m_particleTimer; // how often particles spawn

};