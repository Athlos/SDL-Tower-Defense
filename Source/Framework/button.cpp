#include "button.h"
#include "backbuffer.h"
#include "sprite.h"

#include <cassert>

Button::Button(std::string text)
{
	m_colour = { 0, 0, 0, 0 };
	m_textTexture = NULL;
	m_requiredUpdate = false;
	SetText(text);
	SetBounds(0, 0, 100, 30);

	TTF_Init();
	m_fontSize = 24;
	m_font = TTF_OpenFont("assets/currentfont.TTF", m_fontSize);

	m_backgroundColour = { 192, 192, 192 };
	m_customSprite = false;
}

Button::~Button()
{
}

void Button::Draw(BackBuffer& backBuffer)
{
	if (!m_drawable)
	{
		return;
	}

	backBuffer.SetDrawColour(m_backgroundColour.r, m_backgroundColour.g, m_backgroundColour.b);
	backBuffer.DrawRectangle(&m_bounds, 1);

	if (m_customSprite)
	{
		assert(m_sprite);

		backBuffer.DrawSprite(*m_sprite);
	}
	else
	{
		Label::Draw(backBuffer);
	}
}

void Button::SetCustomSprite(Sprite* sprite)
{
	assert(sprite);

	m_sprite = sprite;

	m_sprite->SetX(m_bounds.x);
	m_sprite->SetY(m_bounds.y);

	m_sprite->SetWidth(m_bounds.w);
	m_sprite->SetHeight(m_bounds.h);

	m_customSprite = true;
}

void Button::SetBackgroundColour(SDL_Color colour)
{
	m_backgroundColour = colour;
}

void Button::SetBackgroundColour(Uint8 r, Uint8 g, Uint8 b)
{
	m_backgroundColour = { r, g, b };
}

Sprite* Button::GetCustomSprite()
{
	return m_sprite;
}