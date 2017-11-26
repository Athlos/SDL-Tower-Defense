#include "label.h"
#include "entity.h"
#include "sprite.h"
#include "backbuffer.h"
#include "texture.h"

#include <cassert>

Label::Label(std::string text)
	: m_textTexture(NULL)
	, m_requiredUpdate(false)
	, m_fontSize(24)
	, m_maxFontSize(24)
	, m_textAlignment(LEFT)
	, m_drawable(true)
{
	m_colour = { 0, 0, 0, 0 };
	SetText(text);
	SetBounds(0, 0, 100, 30);

	TTF_Init();
	m_font = TTF_OpenFont("assets/currentfont.TTF", m_fontSize);
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

void Label::SetText(std::string textOnScreen, BackBuffer& backbuffer)
{
	if (m_text == textOnScreen) 
	{
		return;
	}

	m_text = textOnScreen;
	m_textTexture = backbuffer.CreateText(textOnScreen, m_colour);


	TTF_SizeText(m_font, m_text.c_str(), &m_bounds.w, &m_bounds.h);
}

void Label::SetText(std::string textOnScreen)
{
	if (m_text == textOnScreen)
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
	m_requiredUpdate = true;

	//Debug
	//std::string message;
	//for each (char c in textOnScreen)
	//{
	//	message += c;
	//	if (c == '\n')
	//	{
	//		m_textArray.push_back(message);
	//		message = "";
	//		SDL_Log("NEWLINE");
	//	}
	//}

	//m_textArray.push_back(message);
}

void Label::Draw(BackBuffer& backBuffer)
{
	//Make sure message is synced to the texture
	if (m_requiredUpdate)
	{
		m_requiredUpdate = false;
		m_textTexture = backBuffer.CreateText(m_text, m_colour);

		ResizeText();
	}

	if (m_drawable)
	{
		backBuffer.DrawText(m_textTexture, m_currentBounds);
	}
	//m_textTexture = TTF_RenderText_Blended_Wrapped(m_font, "this is \n 2 lines", m_colour, 50);
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
}

bool Label::WasClickedOn(int x, int y)
{
	if (!m_drawable)
	{
		return false;
	}

	return ((x > m_bounds.x) && (x < m_bounds.x + m_bounds.w) && (y > m_bounds.y) && (y < m_bounds.y + m_bounds.h));
}

void Label::SetFontSize(int size)
{
	m_fontSize = size;
	m_maxFontSize = m_fontSize;

	m_font = TTF_OpenFont("assets/currentfont.TTF", m_fontSize);
}

void Label::SetTextAlignment(Alignment align)
{
	m_textAlignment = align;

	m_requiredUpdate = true;
}

void Label::SetDrawable(bool draw)
{
	m_drawable = draw;
}

void Label::ResizeText()
{
	//Resize bounds to fit text;
	m_currentBounds = m_bounds;

	TTF_SizeText(m_font, m_text.c_str(), &m_currentBounds.w, &m_currentBounds.h);

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

			m_font = TTF_OpenFont("assets/currentfont.TTF", m_fontSize);

			TTF_SizeText(m_font, m_text.c_str(), &m_currentBounds.w, &m_currentBounds.h);
		}
	}
	else if(m_currentBounds.w * 0.85f < m_bounds.w)
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

			m_font = TTF_OpenFont("assets/currentfont.TTF", m_fontSize);

			int newWidth;
			int newHeight;

			TTF_SizeText(m_font, m_text.c_str(), &newWidth, &newHeight);

			if (newWidth > m_bounds.w)
			{
				newWidth = m_bounds.w;
			}

			if (newHeight > m_bounds.h)
			{
				newHeight = m_bounds.h;
			}

			m_currentBounds.w = newWidth;
			m_currentBounds.h = newHeight;
		}
	}

	if (m_textAlignment == CENTER)
	{
		m_currentBounds.x = m_bounds.x + (float)(m_bounds.w - m_currentBounds.w) / 2.0f;
	}
}
