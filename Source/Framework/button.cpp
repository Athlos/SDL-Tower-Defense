#include "button.h"
#include "backbuffer.h"
#include "sprite.h"
#include "game.h"

#include <cassert>

Button::Button(std::string text, BackBuffer& backBuffer) : Label::Label(text, backBuffer)
{
	SetBackgroundColour( 192, 192, 192 );

	UpdateTexture();

	SetTextAlignment(CENTER);

	m_customSprite = false;
	m_sprite = 0;
	m_active = false;
	m_borderWidth = 3;
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

	backBuffer.SetDrawColour(m_borderColour.r, m_borderColour.g, m_borderColour.b);
	backBuffer.DrawRectangle(&m_border, 1);

	if (m_active)
	{
		backBuffer.SetDrawColour(m_highlightColour.r, m_highlightColour.g, m_highlightColour.b);
	}
	else
	{
		backBuffer.SetDrawColour(m_backgroundColour.r, m_backgroundColour.g, m_backgroundColour.b);
	}

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
	m_highlightColour = { (Uint8)(m_backgroundColour.r + 25), (Uint8)(m_backgroundColour.g + 25), (Uint8)(m_backgroundColour.b + 25) };
	m_borderColour = { (Uint8)(m_backgroundColour.r * 0.6), (Uint8)(m_backgroundColour.g * 0.6), (Uint8)(m_backgroundColour.b * 0.6) };
}

void Button::SetBackgroundColour(Uint8 r, Uint8 g, Uint8 b)
{
	SetBackgroundColour({ r, g, b });
}

void Button::SetBounds(int x, int y, int w, int h)
{
	m_border = { x, y, w, h };

	m_bounds = { m_border.x + m_borderWidth, m_border.y + m_borderWidth, m_border.w - (m_borderWidth * 2), m_border.h - (m_borderWidth * 2) };
}

bool Button::WasClickedOn(int x, int y)
{
	if (!m_drawable)
	{
		return false;
	}

	bool result = ((x > m_bounds.x) && (x < m_bounds.x + m_bounds.w) && (y > m_bounds.y) && (y < m_bounds.y + m_bounds.h));

	if (result)
	{
		if (!m_active)
		{
			Game::GetInstance().PlaySound("assets//audio//button_click.wav");
		}

		m_active = true;
	}

	return result;
}

Sprite* Button::GetCustomSprite()
{
	return m_sprite;
}

void Button::SetActive(bool active)
{
	m_active = active;
}
