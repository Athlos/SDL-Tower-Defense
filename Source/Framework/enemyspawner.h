#pragma once

#include <queue>

class Enemy;
class Game;
class BackBuffer;
class Position;

class EnemySpawner
{
	//Member Methods:
public:
	EnemySpawner(float spawnInterval, BackBuffer* backBuffer);
	~EnemySpawner();

	void Process(float deltaTime); // Process the enemies to spawn

	void LoadWave(); // Loads up a wave into a queue to spawn once the wave is active

	int GetWaveNumber() const; // Gets the current wave number
	int GetTotalWaveNumber() const; // Gets the total number of waves

	bool IsWaveActive() const; // Get if the wave is currently active
	void StartWave(std::queue<Position*> path); // Starts spawning enemies in the wave
	void EndWave(); // Ends the wave

	bool WaveSpawned() const; // Return true if there are no enemies left to spawn this wave

	bool AllWavesCompleted() const; // Return true if the end of the game is reached due to all waves being defeated

	//Member Data:
private:
	std::queue<Enemy*> m_enemiesToSpawn; // Queue of enemies to spawn for the current wave
	bool m_waveLoaded; // Flag if the enemies are all ready to spawn, or have already spawned for the current wave

	float m_spawnInterval; // Seconds between each enemy spawned
	float m_timePassed; // Keeps record of time passed for the spawn interval

	bool m_waveActive; // Flag for wave active, while wave is active, enemies will be spawned from the queue
	int m_waveNumber; // Current wave that is either active or loaded
	int m_maxWaves; // Total amount of ways that can spawn

	bool m_allWavesCompleted;

	//Data needed for enemies that will be spawned
	std::queue<Position*> m_mapPath; // Path to set for the enemies as they spawn;
	BackBuffer* m_backBuffer; // Backbuffer required to create enemy sprites
};

