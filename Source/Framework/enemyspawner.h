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

	void Process(float deltaTime);

	void LoadWave();

	int GetWaveNumber() const;

	bool IsWaveActive() const;
	void StartWave(std::queue<Position*> path);
	void EndWave();

	//Member Data:
private:
	std::queue<Enemy*> m_enemiesToSpawn; // Queue of enemies to spawn for the current wave
	bool m_waveLoaded; // Flag if the enemies are all ready to spawn, or have already spawned for the current wave

	float m_spawnInterval; // Seconds between each enemy spawned
	float m_timePassed; // Keeps record of time passed for the spawn interval

	bool m_waveActive; // Flag for wave active, while wave is active, enemies will be spawned from the queue
	int m_waveNumber; // Current wave that is either active or loaded

	//Data needed for enemies that will be spawned
	std::queue<Position*> m_mapPath; // Path to set for the enemies as they spawn;
	BackBuffer* m_backBuffer; // Backbuffer required to create enemy sprites
};

