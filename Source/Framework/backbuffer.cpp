// This include:
#include "backbuffer.h"

// Local includes:
#include "logmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "texture.h"
#include "animatedsprite.h"

#include <stdio.h>

#include <stdlib.h>

#include <string>

#include <vector>

#include <time.h>


// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>
#include <cstring>

using namespace std;

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_width(0)
, m_height(0)
, m_clearRed(0xFF)
, m_clearGreen(0xFF)
, m_clearBlue(0xFF)
{

}

BackBuffer::~BackBuffer()
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = 0;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = 0;

	delete m_pTextureManager;
	m_pTextureManager = 0;

	SDL_FreeSurface(m_surface);
	TTF_CloseFont(m_font);

	//delete m_surface;
	m_surface = 0;

	//delete m_font;
	m_font = 0;

	IMG_Quit();
	SDL_Quit();
}

AnimatedSprite* BackBuffer::CreateAnimatedSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pSprite = new AnimatedSprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

void BackBuffer::DrawAnimatedSprite(AnimatedSprite& sprite, int x, int width) 
{
	SDL_Rect dest;

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = width;
	dest.h = sprite.GetHeight();

	SDL_Rect* f = new SDL_Rect;
	//location of animation frame
	f->x = x;
	f->y = 0;
	f->w = width;
	f->h = sprite.GetHeight();
	
	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), f, &dest);
}

bool BackBuffer::Initialise(int width, int height)
{
	m_width = width;
	m_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("SDL Core Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

		if (m_pWindow == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED); 

			if (m_pRenderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
			}
		}
	}

	m_pTextureManager = new TextureManager();
	assert(m_pTextureManager);
	m_pTextureManager->Initialise(m_pRenderer);

	m_surface = SDL_GetWindowSurface(m_pWindow);

	// Load fonts
	TTF_Init();
	m_font = TTF_OpenFont("assets\\currentfont.ttf", 12);
	
	SDL_Color clrFg = { 0, 0, 255, 0 };  // Blue ("Fg" is foreground)

	//Cursor settings
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	//SDL_WarpMouseInWindow(m_pWindow, 400, 300);

	return (true);
}

void BackBuffer::SetMouse(int x, int y) {
	SDL_WarpMouseInWindow(m_pWindow, x, y);
}

void BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
	SDL_RenderClear(m_pRenderer);
}

void BackBuffer::Present()
{
	SDL_RenderPresent(m_pRenderer);
}

void BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}

void BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}

void BackBuffer::DrawSprite(Sprite& sprite)
{
	SDL_Rect dest;

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = sprite.GetWidth();
	dest.h = sprite.GetHeight();

	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), 0, &dest);
}

void BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2, bool fill)
{
	SDL_Rect fillRect;
	
	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	if (fill)
	{
		SDL_RenderFillRect(m_pRenderer, &fillRect);
	}
	else
	{
		SDL_RenderDrawRect(m_pRenderer, &fillRect);
	}
}

void BackBuffer::DrawRectangle(SDL_Rect* bounds, bool fill)
{
	if (fill)
	{
		SDL_RenderFillRect(m_pRenderer, bounds);
	}
	else
	{
		SDL_RenderDrawRect(m_pRenderer, bounds);
	}
}

void BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

Sprite* BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

void BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearRed = r;
	m_clearGreen = g;
	m_clearBlue = b;
}

void BackBuffer::DrawText(SDL_Texture* textOnScreen, SDL_Rect bounds)
{
	//Basic render of the texture of the label
	SDL_RenderCopy(m_pRenderer, textOnScreen, 0, &bounds);
}

SDL_Texture* BackBuffer::CreateText(std::string text, SDL_Color colour)
{
	//replace surface
	//SDL_FreeSurface(m_surface);
	//m_surface = 0;

	//m_surface = SDL_GetWindowSurface(m_pWindow);

	//create text and save into surface, then use surface to create a texture we can render
	m_surface = TTF_RenderText_Blended(m_font, text.c_str(), colour);
	SDL_Texture* tTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	return tTexture;

}