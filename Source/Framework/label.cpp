#include "label.h"
#include "backbuffer.h"

#include <cassert>

Label::Label(std::string text, BackBuffer& backBuffer)
	: m_textTexture(0)
	, m_backBuffer(&backBuffer)
	, m_fontSize(24)
	, m_maxFontSize(24)
	, m_textAlignment(LEFT)
	, m_drawable(true)
{
	SetBounds(0, 0, 100, 30);
	m_colour = { 0, 0, 0, 0 };

	m_fontName = "assets/zekton rg.TTF";

	TTF_Init();
	UpdateFont();

	SetText(text);
}

Label::Label()
{

}

Label::~Label()
{

}

std::string Label::GetText()
{
	return m_text;
}

void Label::SetText(std::string textOnScreen)
{
	if (m_text.compare(textOnScreen) == 0)
	{
		return;
	}

	if (textOnScreen == "")
	{
		m_drawable = false;
	}
	else
	{
		m_drawable = true;
	}

	m_text = textOnScreen;
	UpdateTexture();
}

void Label::Draw(BackBuffer& backBuffer)
{
	if (m_drawable)
	{
		backBuffer.DrawText(m_textTexture, m_currentBounds);
	}
}

SDL_Rect Label::GetBounds()
{
	return m_bounds;
}

void Label::SetBounds(int x, int y, int w, int h)
{
	m_bounds.x = x;
	m_bounds.y = y;
	m_bounds.w = w;
	m_bounds.h = h;

	m_currentBounds = m_bounds;
}

void Label::SetColour(int r, int g, int b, int a)
{
	m_colour = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
	UpdateTexture();
}

void Label::SetFontSize(int size)
{
	m_fontSize = size;
	m_maxFontSize = m_fontSize;

	UpdateFont();

	UpdateTexture();
}

void Label::SetTextAlignment(Alignment align)
{
	m_textAlignment = align;
	UpdateTexture();
}

void Label::SetDrawable(bool draw)
{
	m_drawable = draw;
}

void Label::ResizeText()
{
	//Resize bounds to fit text;
	m_currentBounds = m_bounds;

	TTF_SizeText(m_font, m_text.c_str(), &m_currentBounds.w, 0);

	if (m_currentBounds.w * 1.15f > m_bounds.w)
	{
		while (m_currentBounds.w > m_bounds.w)
		{
			int newFontSize = m_fontSize * 0.9f;

			if (newFontSize == m_fontSize)
			{
				--newFontSize;
			}

			m_fontSize = newFontSize;

			UpdateFont();

			TTF_SizeText(m_font, m_text.c_str(), &m_currentBounds.w, 0);
		}
	}
	else if (m_currentBounds.w * 0.85f < m_bounds.w)
	{
		while (m_currentBounds.w < m_bounds.w && m_drawable && m_fontSize != m_maxFontSize)
		{
			int newFontSize = m_fontSize * 1.1f;

			if (newFontSize == m_fontSize)
			{
				++newFontSize;
			}

			m_fontSize = newFontSize;

			if (m_fontSize > m_maxFontSize)
			{
				m_fontSize = m_maxFontSize;
			}

			UpdateFont();

			int newWidth;

			TTF_SizeText(m_font, m_text.c_str(), &newWidth, 0);

			if (newWidth > m_bounds.w)
			{
				newWidth = m_bounds.w;
			}

			m_currentBounds.w = newWidth;
		}
	}

	if (m_textAlignment == CENTER)
	{
		m_currentBounds.x = m_bounds.x + (float)(m_bounds.w - m_currentBounds.w) / 2.0f;
	}
}

void Label::UpdateTexture()
{
	ResizeText();

	SDL_DestroyTexture(m_textTexture);
	m_textTexture = 0;

	m_textTexture = m_backBuffer->CreateText(m_text, m_colour, m_font);
}

void Label::UpdateFont()
{
	m_font = TTF_OpenFont(m_fontName.c_str(), m_fontSize);

}
