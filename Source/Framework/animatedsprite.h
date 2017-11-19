#pragma once

#include "sprite.h"
#include "SDL.h"

#include <vector>

class BackBuffer;
class Texture;

class AnimatedSprite : public Sprite
{
	// Member Methods:
public:
	AnimatedSprite();
	~AnimatedSprite();

	bool Initialise(Texture& texture); // Initialise sprite texture

	void Process(float deltaTime); // Process animated sprite frames and timings
	void Draw(BackBuffer& backbuffer); // Draw current frame

	void AnimatedSprite::LoadFrames(int width, int height); // Load up frame coordinates in current sprite sheet

	void SetFrameSpeed(float f); // Set duration between switching frames
	void SetFrameWidth(int w); // Set width of a single frame in the sprite sheet

	void Pause(); // Pause processing through the animation frames
	bool IsPaused(); // Get if animation if paused

	bool IsAnimating(); // Get if sprite is being animated
	void StartAnimating(); // Begin animating the sprite
	void StopAnimating(); // Stop aninating the sprite

	bool IsLooping(); // Get if animation loops
	void SetLooping(bool b); // Set if the animation should loop

	Texture* GetTexture(); // Get sprite sheet

	void SetScale(int width, int height); // Set the bounds each frame should fit in
	int GetFrameCount(); // Get total frames

	// Member Data:
protected:

	std::vector<SDL_Point*> m_totalFrames; // All frame coordinates in the sprite sheet

	float m_frameSpeed; // Time between frames
	int m_frameWidth; // Width of a single frame in sprite sheet

	float m_timeElapsed; // Time passed counter
	int m_currentFrame; // Index of current frame

	bool m_paused; // Paused flag
	bool m_loop; // Looping flag
	bool m_animating; // animating flag

	int m_scaleWidth; // Width to strech the sprite to
	int m_scaleHeight; // Height to strech the sprite to
};