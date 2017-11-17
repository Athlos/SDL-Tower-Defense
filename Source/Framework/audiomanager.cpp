#include "audiomanager.h"

#include <cassert>

AudioManager::AudioManager()
{
	m_system = 0;

	m_result = FMOD::System_Create(&m_system);      // Create the main system object.
	m_result = m_system->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.

	m_mainChannel = 0;
	m_musicChannel = 0;
}

AudioManager::~AudioManager()
{
	m_system->release();
}

void AudioManager::Process(float deltaTime)
{
	m_system->update(); // Update system
}

bool AudioManager::PlaySound(std::string filename)
{
	FMOD::Sound* newSound = GetSound(filename);

	if (newSound != 0)
	{
		m_result = m_system->playSound(newSound, 0, false, &m_mainChannel);
	}

	return false;
}

FMOD::Sound * AudioManager::GetSound(std::string filename)
{
	FMOD::Sound* newSound = 0;

	if (m_loadedSounds.find(filename) == m_loadedSounds.end()) // Check if sounds has not been loaded
	{
		m_result = m_system->createSound(filename.c_str(), FMOD_DEFAULT, 0, &newSound);

		newSound->setMode(FMOD_LOOP_OFF);

		m_loadedSounds[filename] = newSound;
	}
	else
	{
		newSound = m_loadedSounds[filename];
	}

	assert(newSound != 0);

	return newSound;
}
