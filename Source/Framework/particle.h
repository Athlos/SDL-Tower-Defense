#pragma once

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

	bool Initialise(Sprite* sprite); // Initialise a particle sprite

	void Process(float deltaTime); // Process the sprite positions
	void Draw(BackBuffer& backbuffer); // Draw the sprite

	void SetAccelerationX(float x); // Set the x acceleration
	void SetAccelerationY(float y); // Set the y acceleration

	float GetAge(); // Get the time since particle has spawned
	void SetAge(float age); // Set the time since particle spawned

	void SpawnParticle(int x, int y, BackBuffer* backbuffer); // Spawn the particle in a specific position
	ParticleType GetType(); // Gets the type of particle

	// Member Data:
protected:
	float m_accelerationX; // X acceleration, set to negative to have the particle slow down
	float m_accelerationY; // Y acceleration, set to negative to have the particle slow down

	float m_age; // Duration since particle creation
	ParticleType m_type; // Type of particle

};