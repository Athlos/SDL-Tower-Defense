#include "enemyspawner.h"
#include "game.h"
#include "enemy.h"
#include "backbuffer.h"

EnemySpawner::EnemySpawner(float spawnInterval, BackBuffer* backBuffer)
{
	m_spawnInterval = spawnInterval;

	m_timePassed = 0;
	m_waveActive = false;
	m_waveLoaded = false;
	m_waveNumber = 1;

	m_backBuffer = backBuffer;
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::Process(float deltaTime)
{
	if (m_waveActive && !m_enemiesToSpawn.empty())
	{
		m_timePassed += deltaTime;

		if (m_timePassed >= m_spawnInterval)
		{
			m_timePassed -= m_spawnInterval;

			Enemy* spawn = m_enemiesToSpawn.front();
			m_enemiesToSpawn.pop();

			spawn->SetPath(m_mapPath);

			Game::GetInstance().AddEnemy(spawn);
		}
	}
}

void EnemySpawner::LoadWave()
{
	if (m_waveLoaded)
	{
		return;
	}

	//TODO
	m_waveLoaded = true;

	for (int i = 0; i < m_waveNumber * 3; ++i)
	{
		int type = rand() % 3;

		Enemy* testEnemy = new Enemy();
		Sprite* testEnemySprite;

		switch (type)
		{
			case 0: // Large enemy, high health and low speed
			{
				testEnemySprite = m_backBuffer->CreateSprite("assets\\enemy.png");
				testEnemy->SetData(7 * (m_waveNumber / 2.0f), rand() % 31 + 20, 1);
			}
				break;
			case 1: // Medium enemy, normal health, normal speed
			{
				testEnemySprite = m_backBuffer->CreateSprite("assets\\enemy_medium.png");
				testEnemy->SetData(5 * (m_waveNumber / 2.0f), rand() % 51 + 50, 1);
			}
				break;
			case 2: // Small enemy, low health, high speed
			{
				testEnemySprite = m_backBuffer->CreateSprite("assets\\enemy_small.png");
				testEnemy->SetData(3 * (m_waveNumber / 2.0f), rand() % 51 + 100, 1);
			}
				break;
			default:
			{
				testEnemySprite = m_backBuffer->CreateSprite("assets\\enemy.png");
				testEnemy->SetData(7 * (m_waveNumber / 2.0f), rand() % 31 + 20, 1);
			}
				break;
		}

		testEnemy->Initialise(testEnemySprite);

		testEnemy->SetPosition(1, 1);

		m_enemiesToSpawn.push(testEnemy);
	}
}

int EnemySpawner::GetWaveNumber() const
{
	return m_waveNumber;
}

bool EnemySpawner::IsWaveActive() const
{
	return m_waveActive;
}

void EnemySpawner::StartWave(std::queue<Position*> path)
{
	if (!m_waveLoaded)
	{
		LoadWave();
	}

	m_mapPath = path;

	m_waveActive = true;

	m_timePassed = m_spawnInterval;
}

void EnemySpawner::EndWave()
{
	m_waveActive = false;

	++m_waveNumber;
	
	m_waveLoaded = false;

	LoadWave();
}
