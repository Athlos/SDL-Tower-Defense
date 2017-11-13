// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"

AnimatedSprite::AnimatedSprite()
	: m_frameSpeed(0.0f)
	, m_frameWidth(0)
	, m_timeElapsed(0.0f)
	, m_currentFrame(0)
	, m_paused(false)
	, m_loop(false)
	, m_animating(false)
{
}

AnimatedSprite::~AnimatedSprite()
{
}

Texture* AnimatedSprite::GetTexture()
{
	return (m_pTexture);
}


bool AnimatedSprite::Initialise(Texture& texture)
{
	m_frameWidth = 0;
	m_frameSpeed = 0;

	m_loop = false;
	m_paused = false;
	m_animating = true;

	Sprite::Initialise(texture);
	StartAnimating();

	return (true);
}

void AnimatedSprite::AddFrame(int x)
{
	//Add the x coordinate to the frame coordinate container.
	m_totalFrames.push_back(x);
}

void AnimatedSprite::Process(float deltaTime)
{
	//If not paused...
	if (m_paused) {
		return;
	}
	//Count the time elapsed.
	m_timeElapsed += deltaTime;
	//If the time elapsed is greater than the frame speed.
	if (m_timeElapsed > m_frameSpeed) 
	{
		//reset if its at max frames
		if (m_currentFrame >= m_totalFrames.size()-1) 
		{
			//if looping set to 0
			if (m_loop)
			{
				m_currentFrame = 0;
			}
			else 
			{
			//if not looping turn of animation
				m_animating = false;
			}
		}
		//if not at max frames add 1
		if (m_currentFrame < m_totalFrames.size() - 1)
		{
			m_currentFrame++;
		}
		m_timeElapsed = 0.0f;
	}
}

void AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	//Draw the particular frame into the backbuffer.
	backbuffer.DrawAnimatedSprite(*this, m_totalFrames[m_currentFrame], m_frameWidth);
}

void AnimatedSprite::SetFrameSpeed(float f)
{
	m_frameSpeed = f;
}

void AnimatedSprite::SetFrameWidth(int w)
{
	m_frameWidth = w;
}

void AnimatedSprite::Pause()
{
	m_paused = !m_paused;
}

bool AnimatedSprite::IsPaused()
{
	return (m_paused);
}

bool AnimatedSprite::IsAnimating()
{
	return (m_animating);
}

void AnimatedSprite::StartAnimating()
{
	m_animating = true;

	m_timeElapsed = 0;
	m_currentFrame = 0;
}

void AnimatedSprite::StopAnimating()
{
	m_animating = false;
}

bool AnimatedSprite::IsLooping()
{
	return (m_loop);
}

void AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}