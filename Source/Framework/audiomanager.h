#pragma once

#include "fmod.hpp";

#include <string>
#include <map>

class AudioManager
{
	//Member Methods:
public:
	AudioManager();
	~AudioManager();

	void Process(float deltaTime); // Process fmod system

	bool PlaySound(std::string filename); // Play a sound with a filename
	FMOD::Sound* GetSound(std::string filename); // Get a sound with a filename

	//Member Data:
protected:
	FMOD::System* m_system; // FMOD system
	FMOD_RESULT m_result; // FMOD result

	FMOD::Channel* m_mainChannel; // Main channel for audio
	FMOD::Channel* m_musicChannel; // Music channel for audio

	std::map<std::string, FMOD::Sound*> m_loadedSounds; // All stored sounds requested so far
};

