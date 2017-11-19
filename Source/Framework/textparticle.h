#pragma once

#include "particle.h"
#include <string>

class Label;

class TextParticle : public Particle
{
	// Member Methods:
public:
	TextParticle();
	~TextParticle();

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SpawnParticle(int x, int y, std::string message); // Spawn a text particle

	// Member Data:
protected:
	Label* m_text;
};

