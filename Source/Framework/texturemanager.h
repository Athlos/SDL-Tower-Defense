#pragma once

// Library Includes:
#include <string>
#include <map>

// Forward Declarations:
class Texture;
struct SDL_Renderer;

class TextureManager
{
	//Member Methods:
public:
	TextureManager();
	~TextureManager();

	bool Initialise(SDL_Renderer* pRenderer);

	Texture* GetTexture(const char* pcFilename); // Get a texture from a file name. If it exists, get from map, otherwise load it from file

private:
	TextureManager(const TextureManager& textureManager);
	TextureManager& operator=(const TextureManager& textureManager);

	//Member Data:
protected:
	SDL_Renderer* m_pRenderer; // Renderer

	std::map<std::string, Texture*> m_pLoadedTextures; // Map of textures using filename as key

};