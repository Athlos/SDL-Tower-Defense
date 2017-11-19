#pragma once

// Library includes:
#include <SDL.h>

class Texture
{
	//Member Methods:
public:
	Texture();
	~Texture();

	bool Initialise(const char* pcFilename, SDL_Renderer* pRenderer);

	int GetWidth() const; // Get the texture width
	int GetHeight() const; // Get the texture height

	SDL_Texture* GetTexture(); // Get the SDL texture being used

private:
	Texture(const Texture& texture);
	Texture& operator=(const Texture& texture);

	//Member Data:
protected:
	SDL_Renderer* m_pRenderer; // Renderer
	SDL_Texture* m_pTexture; // SDL texture

	int m_width; // Texture width
	int m_height; // Texture height

	SDL_RendererFlip m_flip; // Render flip

private:

};