#include "enemyspawner.h"
#include "game.h"
#include "enemy.h"
#include "backbuffer.h"
#include "animatedsprite.h"

EnemySpawner::EnemySpawner(float spawnInterval, BackBuffer* backBuffer)
	: m_spawnInterval(spawnInterval)
	, m_timePassed(0)
	, m_waveActive(false)
	, m_waveLoaded(false)
	, m_waveNumber(false)
	, m_maxWaves(5)
	, m_backBuffer(backBuffer)
	, m_allWavesCompleted(false)
{
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

	for (int i = 0; i < m_waveNumber * 4; ++i)
	{
		int type = rand() % 3;

		Enemy* testEnemy = new Enemy();
		AnimatedSprite* testEnemySprite;

		switch (type)
		{
			case 0: // Large enemy, high health and low speed
			{
				testEnemySprite = m_backBuffer->CreateAnimatedSprite("assets\\enemy_tank.png");
				testEnemy->SetData(18 * m_waveNumber, rand() % 31 + 20, 1);
				testEnemySprite->SetScale(48, 48);
			}
				break;
			case 1: // Medium enemy, normal health, normal speed
			{
				testEnemySprite = m_backBuffer->CreateAnimatedSprite("assets\\enemy_minigun.png");
				testEnemy->SetData(15 * m_waveNumber, rand() % 51 + 50, 1);
				testEnemySprite->SetScale(48, 48);
			}
				break;
			case 2: // Small enemy, low health, high speed
			{
				testEnemySprite = m_backBuffer->CreateAnimatedSprite("assets\\enemy_drone.png");
				testEnemy->SetData(13 * m_waveNumber, rand() % 51 + 100, 1);
				testEnemySprite->SetScale(48, 48);
			}
				break;
			default:
			{
				testEnemySprite = m_backBuffer->CreateAnimatedSprite("assets\\enemy_spider.png");
				testEnemy->SetData(7 * m_waveNumber, rand() % 31 + 20, 1);
				testEnemySprite->SetScale(32, 32);
			}
				break;
		}

		testEnemySprite->LoadFrames(32, 32);

		testEnemy->Initialise(testEnemySprite);

		testEnemy->SetPosition(1, 1);

		m_enemiesToSpawn.push(testEnemy);
	}
}

int EnemySpawner::GetWaveNumber() const
{
	return m_waveNumber;
}

int EnemySpawner::GetTotalWaveNumber() const
{
	return m_maxWaves;
}

bool EnemySpawner::IsWaveActive() const
{
	return m_waveActive;
}

void EnemySpawner::StartWave(std::queue<Position*> path)
{
	if (m_allWavesCompleted)
	{
		return;
	}

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

	if (m_waveNumber == m_maxWaves)
	{
		m_allWavesCompleted = true;
	}
	else
	{
		++m_waveNumber;

		m_waveLoaded = false;

		LoadWave();

	}
}

bool EnemySpawner::WaveSpawned() const
{
	return m_enemiesToSpawn.empty();
}

bool EnemySpawner::AllWavesCompleted() const
{
	return m_allWavesCompleted;
}
