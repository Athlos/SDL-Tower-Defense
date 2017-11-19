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
	, m_scaleWidth(0)
	, m_scaleHeight(0)
{
}

AnimatedSprite::~AnimatedSprite()
{
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
		if (m_currentFrame >= m_totalFrames.size() - 1)
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
	SDL_Rect base{ m_totalFrames[m_currentFrame]->x, m_totalFrames[m_currentFrame]->y, m_width, m_height };
	SDL_Rect scale{ m_x, m_y, m_scaleWidth, m_scaleHeight };

	backbuffer.DrawAnimatedSprite(*this, &base, &scale);
}

void AnimatedSprite::LoadFrames(int width, int height)
{
	//Set the center to half the width and height
	SetCenter(width / 2, height / 2);

	//Set width and height to the same value, assuming your sprite is a square
	m_width = width;
	m_height = height;

	//default frame speed
	m_frameSpeed = 0.1f;

	//loops by default
	m_loop = true;
	for (int a = 0; a < m_pTexture->GetHeight(); a += height)
	{
		//Grab the texture, and grab frames the size of the width for 1 row
		for (int i = 0; i < m_pTexture->GetWidth(); i += width)
		{
			//Store frame coordinates to render later
			SDL_Point* newFrame = new SDL_Point();
			newFrame->x = i;
			newFrame->y = a;

			m_totalFrames.push_back(newFrame);
		}
	}
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

Texture* AnimatedSprite::GetTexture()
{
	return (m_pTexture);
}

void AnimatedSprite::SetScale(int width, int height)
{
	m_scaleWidth = width;
	m_scaleHeight = height;
}

int AnimatedSprite::GetFrameCount()
{
	return m_totalFrames.size();
}

