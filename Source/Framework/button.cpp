#include "button.h"
#include "backbuffer.h"
#include "sprite.h"

#include <cassert>

Button::Button(std::string text)
{
	Label::Label(text);

	m_backgroundColour = { 192, 192, 192 };
}

Button::~Button()
{
}

void Button::Draw(BackBuffer& backBuffer)
{
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

void Button::SetBackgroundColour(int r, int g, int b)
{
	m_backgroundColour = { r, g, b };
}

Sprite* Button::GetCustomSprite()
{
	return m_sprite;
}