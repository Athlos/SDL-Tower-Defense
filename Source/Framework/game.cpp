// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "fmod.hpp"
#include "label.h"
#include "particleemitter.h"
#include "grid.h"
#include "tile.h"
#include "pathfinding.h"
#include "enemy.h"
#include "tower.h"
#include "position.h"
#include "quadtree.h"
#include "enemyspawner.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <vector>

//TESTING
#include <chrono>
#include <sstream>

// Static Members:
Game* Game::sm_pInstance = 0;

Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, m_paused(0)
{

}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_particles;
	m_particles = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	system->release();
}

/*
Initialises renderer, input, audio, player and other variables needed to begin
*/
bool Game::Initialise()
{

	// Load backbuffer
	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(m_screenWidth, m_screenHeight))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	// Load input
	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	// Seed rand
	srand(time(0));

	//SET UP ENTITIES AND VARIABLES
	m_map = new Grid(15, 15);

	m_pathfinding = new Pathfinding();
	m_pathfinding->m_grid = m_map;

	m_map->CreateGrid(*m_pBackBuffer);
	m_map->UpdatePath();

	m_particles = new ParticleEmitter();

	Position* topLeftPos = new Position( m_screenHeight / 2, m_screenHeight / 2 );
	AxisAlignedBoundingBox* gridBounds = new AxisAlignedBoundingBox (topLeftPos, m_screenHeight / 2 );
	m_quadTree = new QuadTree(gridBounds);

	m_totalLives = 100;
	m_currentLives = m_totalLives;

	m_currency = 5000;

	m_enemySpawner = new EnemySpawner(0.5f, m_pBackBuffer);

	//SET UP UI

	m_debug_fps = new Label("");
	m_debug_fps->SetBounds(m_screenWidth - 48, 0, 48, 24);
	m_debug_fps->SetColour(0, 255, 0, 50);

	m_lifeCounter = new Label("");

	m_lifeCounter->SetBounds(m_screenWidth * 0.8f, 30, m_screenWidth * 0.15f, 32);
	m_lifeCounter->SetColour(255, 0, 0, 50);

	UpdateLives(0);

	m_waveCounter = new Label("");

	m_waveCounter->SetBounds(m_screenWidth * 0.8f, 5, m_screenWidth * 0.15f, 25);
	m_waveCounter->SetColour(0, 0, 255, 50);

	UpdateWaves();

	m_currencyCounter = new Label("");

	m_currencyCounter->SetBounds(m_screenWidth * 0.8f, 65, m_screenWidth * 0.15f, 30);
	m_currencyCounter->SetColour(255, 215, 0, 50);

	UpdateCurrency(0);

	//SET UP AUDIO
	system = NULL;

	result = FMOD::System_Create(&system);      // Create the main system object.
	result = system->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.

	channel = 0;
	combatMusic = 0;
	m_musicChannel = 0;

	// Timings
	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool Game::DoGameLoop()
{
	//struct coord{
	//	int x1;
	//	int y1;
	//	int x2;
	//	int y2;
	//};

	//coord coordinates[250];

	//for (int i = 0; i < 250; ++i)
	//{
	//	coordinates[i].x1 = rand() % 800;
	//	coordinates[i].y1 = rand() % 600;

	//	coordinates[i].x2 = rand() % 800;
	//	coordinates[i].y2 = rand() % 600;
	//}

	//auto startVec = std::chrono::system_clock::now();

	//for (int i = 0; i < 250; ++i)
	//{
	//	m_pathfinding->FindPathVector(coordinates[i].x1, coordinates[i].y1, coordinates[i].x2, coordinates[i].y2);
	//}

	//auto endVec = std::chrono::system_clock::now();

	//std::chrono::duration<double> elapsed_secondsVec = endVec - startVec;



	//auto startHeap = std::chrono::system_clock::now();

	//for (int i = 0; i < 250; ++i)
	//{
	//	m_pathfinding->FindPath(coordinates[i].x1, coordinates[i].y1, coordinates[i].x2, coordinates[i].y2);
	//}

	//auto endHeap = std::chrono::system_clock::now();

	//std::chrono::duration<double> elapsed_secondsHeap = endHeap - startHeap;

	const float stepSize = 1.0f / 60.0f; // calculate step size

	// Check input
	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	if (m_looping)
	{
		// Set up time values
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize); // Process

			m_lag -= stepSize;

			++m_numUpdates;
		}

		Draw(*m_pBackBuffer); // Render
	}

	SDL_Delay(1);

	return (m_looping);
}

void Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;

		//Update fps label
		std::string fpsString = std::to_string(m_FPS);
		m_debug_fps->SetText(fpsString);
	}

	// Check if game is paused
	if (m_paused)
	{
		return;
	}

	m_particles->Process(deltaTime); // Process particles

	m_enemySpawner->Process(deltaTime);

	std::vector<Enemy*>::iterator enemyIter = m_enemies.begin();

	while (enemyIter != m_enemies.end())
	{
		Enemy* current = *enemyIter;

		if (current->IsDead() || current->ReachedEnd())
		{
			if (current->ReachedEnd())
			{
				UpdateLives(-1);
			}
			else
			{
				UpdateCurrency(current->GetReward());
			}

			enemyIter = m_enemies.erase(enemyIter);

			delete current;
			current = 0;

			if (m_enemies.empty())
			{
				m_enemySpawner->EndWave();
				UpdateWaves();
			}
		}
		else
		{
			current->Process(deltaTime);
			current->m_targetted = false;
			enemyIter++;
		}
	}

	//Redo quad tree
	delete(m_quadTree);

	Position* topLeftPos = new Position(m_screenHeight / 2, m_screenHeight / 2);
	AxisAlignedBoundingBox* gridBounds = new AxisAlignedBoundingBox(topLeftPos, m_screenHeight / 2);
	m_quadTree = new QuadTree(gridBounds);

	for each (Enemy* enemy in m_enemies)
	{
		m_quadTree->Insert(enemy);
	}

	for each (Tower* tower in m_towers)
	{
		std::vector<Entity*> entitiesInRange = m_quadTree->QueryRange(tower->GetRangeBounds());

		tower->SetEnemiesInRange(entitiesInRange);

		for each (Enemy* enemy in entitiesInRange)
		{
			enemy->m_targetted = true;
		}

		tower->Process(deltaTime);
	}

	system->update(); // Update system
}

void Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	m_particles->Draw(backBuffer);

	m_map->Draw(backBuffer);

	for each (Enemy* enemy in m_enemies)
	{
		enemy->Draw(backBuffer);
	}

	for each (Tower* tower in m_towers)
	{
		tower->Draw(backBuffer);
	}

	DrawUI(backBuffer);

	//m_quadTree->Draw(backBuffer);

	backBuffer.Present();
}

void Game::DrawUI(BackBuffer& backBuffer)
{
	//Draw UI
	//m_debug_fps->Draw(backBuffer);
	m_lifeCounter->Draw(backBuffer);
	m_waveCounter->Draw(backBuffer);
	m_currencyCounter->Draw(backBuffer);
}

void Game::UpdateLives(int amount)
{
	m_currentLives += amount;

	std::stringstream lifeMessage;
	lifeMessage << "Lives: " << m_currentLives;

	m_lifeCounter->SetText(lifeMessage.str());
}

void Game::UpdateWaves()
{
	std::stringstream waveMessage;
	waveMessage << "Wave: " << m_enemySpawner->GetWaveNumber();
	m_waveCounter->SetText(waveMessage.str());
}

void Game::UpdateCurrency(int amount)
{
	m_currency += amount;

	std::stringstream currencyMessage;
	currencyMessage << "$" << m_currency;
	m_currencyCounter->SetText(currencyMessage.str());
}

void Game::Quit()
{
	m_looping = false;
}

void Game::Pause(bool pause)
{
	m_paused = pause;
	m_musicChannel->setPaused(pause);
}

bool Game::IsPaused()
{
	return (m_paused);
}

void Game::OnLeftMouseClick(int x, int y)
{
	//TestQuadTree(x, y);
	//return;

	if (m_enemySpawner->IsWaveActive())
	{
		for each (Enemy* e in m_enemies)
		{
			if (e->IsClickedOn(x, y))
			{
				e->TakeDamage(1);
			}
		}
	}
	else
	{
		Tile* clicked = m_map->GetTileFromPixelCoord(x, y);

		if (clicked != 0)
		{
			if (clicked->GetState() == EMPTY && m_currency >= 10)
			{
				clicked->SetWall(true);
				clicked->SetOccupied(true);

				//TODO stop tile placement if path is obstructed

				if (!m_map->UpdatePath())
				{
					clicked->SetOccupied(false);
					clicked->SetWall(false);
				}
				else
				{
					UpdateCurrency(-10);
				}
			}
		}
	}
}

void Game::TestQuadTree(int x, int y)
{
	Position* newPos = new Position(x, y);
	//m_quadTree->Insert(newPos);
}

void Game::OnRightMouseClick(int x, int y)
{
	Tile* clicked = m_map->GetTileFromPixelCoord(x, y);

	if (clicked->GetState() == BLOCKED)
	{
		clicked->SetState(EMPTY);
	}
}

void Game::StartWave()
{
	m_map->UpdatePath();

	std::queue<Position*> path = m_pathfinding->SimplifyPath(m_map->GetGridPath());

	m_enemySpawner->StartWave(path);
}

void Game::SpawnEnemies(int amount)
{
	for (int i = 0; i < amount; ++i)
	{

	}
}

void Game::AddEnemy(Enemy* enemy)
{
	enemy->SetTilePosition(m_map->GetGridStart());

	enemy->m_grid = m_map;

	m_quadTree->Insert(enemy);

	m_enemies.push_back(enemy);
}

void Game::PlaceTower(int x, int y)
{
	if (m_enemySpawner->IsWaveActive())
	{
		return;
	}

	Tile* currentTile = m_map->GetTileFromPixelCoord(x, y);

	if (m_currency < 100 || currentTile->GetState() == BLOCKED)
	{
		return;
	}

	currentTile->SetOccupied(true);

	if (!m_map->UpdatePath())
	{
		currentTile->SetOccupied(false);

		return;
	}

	Tower* newTower = new Tower(1, 1, 1, 100);

	Sprite* newTowerSprite = m_pBackBuffer->CreateSprite("assets\\tower_base.png");

	newTower->Initialise(newTowerSprite);

	//Scale to tile size
	newTower->GetSprite()->SetWidth(currentTile->GetTileWidth());
	newTower->GetSprite()->SetHeight(currentTile->GetTileHeight());

	newTower->SetTilePosition(currentTile);

	

	m_towers.push_back(newTower);

	m_map->UpdatePath();

	UpdateCurrency(-newTower->GetTowerCost());
}