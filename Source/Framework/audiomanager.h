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

	void Process(float deltaTime);

	bool PlaySound(std::string filename);
	FMOD::Sound* GetSound(std::string filename);

	//Member Data:
protected:
	//AUDIO
	FMOD::System* m_system;
	FMOD_RESULT m_result;

	FMOD::Channel* m_mainChannel;
	FMOD::Channel* m_musicChannel;

	std::map<std::string, FMOD::Sound*> m_loadedSounds;
};

