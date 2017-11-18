#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

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

	bool Initialise(Texture& texture);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void AnimatedSprite::LoadFrames(int width, int height);

	void SetFrameSpeed(float f);
	void SetFrameWidth(int w);

	void Pause();
	bool IsPaused();

	bool IsAnimating();
	void StartAnimating();
	void StopAnimating();

	bool IsLooping();
	void SetLooping(bool b);

	Texture* GetTexture();

	void SetScale(int width, int height);
	int GetFrameCount();

	// Member Data:
protected:

	std::vector<SDL_Point*> m_totalFrames;

	float m_frameSpeed;
	int m_frameWidth;

	float m_timeElapsed;
	int m_currentFrame;

	bool m_paused;
	bool m_loop;
	bool m_animating;

	int m_scaleWidth;
	int m_scaleHeight;

private:

};

#endif // __ANIMATEDSPRITE_H__