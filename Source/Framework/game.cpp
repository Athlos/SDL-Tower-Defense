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
#include "button.h"
#include "particleemitter.h"
#include "grid.h"
#include "tile.h"
#include "pathfinding.h"
#include "enemy.h"
#include "tower.h"
#include "position.h"
#include "quadtree.h"
#include "enemyspawner.h"
#include "projectile.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <iomanip>

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

	Position* towerTopLeft = new Position(m_screenHeight / 2, m_screenHeight / 2);
	AxisAlignedBoundingBox* towerGridBounds = new AxisAlignedBoundingBox(towerTopLeft, m_screenHeight / 2);
	m_towerQuadTree = new QuadTree(towerGridBounds);

	m_totalLives = 100;
	m_currentLives = m_totalLives;

	m_currency = 500;

	m_enemySpawner = new EnemySpawner(0.5f, m_pBackBuffer);

	m_selectedTower = 0;

	//SET UP UI

	m_debug_fps = new Label("");
	m_debug_fps->SetBounds(m_screenWidth - 48, 0, 48, 24);
	m_debug_fps->SetColour(0, 255, 0, 50);

	m_lifeCounter = new Label("");

	m_lifeCounter->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.01f, m_screenWidth * 0.15f, m_screenHeight * 0.05f);
	m_lifeCounter->SetColour(255, 0, 0, 50);

	UpdateLives(0);

	m_waveCounter = new Label("");

	m_waveCounter->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.07f, m_screenWidth * 0.15f, m_screenHeight * 0.05f);
	m_waveCounter->SetColour(0, 0, 255, 50);

	UpdateWaves();

	m_towerText = new Label("Buildings");

	m_towerText->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.20f, m_screenWidth * 0.23f, m_screenHeight * 0.07f);
	m_towerText->SetColour(255, 0, 0, 50);
	m_towerText->SetFontSize(32);

	m_highlighted = new Label(" Selected");

	m_highlighted->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.52f, m_screenWidth * 0.23f, m_screenHeight * 0.07f);
	m_highlighted->SetColour(240, 230, 140, 50);
	m_highlighted->SetFontSize(32);

	m_wallButton = new Button("");
	m_wallButton->SetBounds(m_screenWidth * 0.77f, m_screenHeight * 0.28f, m_screenWidth * 0.05f, m_screenWidth * 0.05f);
	Sprite* wallSprite = m_pBackBuffer->CreateSprite("assets\\wall_base.png");
	m_wallButton->SetCustomSprite(wallSprite);

	m_towerButton = new Button("");
	m_towerButton->SetBounds(m_screenWidth * 0.84f, m_screenHeight * 0.28f, m_screenWidth * 0.05f, m_screenWidth * 0.05f);

	Sprite* newTowerSprite = m_pBackBuffer->CreateSprite("assets\\tower_base.png");
	m_towerButton->SetCustomSprite(newTowerSprite);

	m_startWave = new Button("Start Wave");
	m_startWave->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.93f, m_screenWidth * 0.23f, m_screenWidth * 0.05f);
	m_startWave->SetFontSize(28);
	m_startWave->SetBackgroundColour(34, 139, 34);
	m_startWave->SetColour(255, 255, 255, 255);

	m_selected = NOTHING;

	m_cursorSprite = 0;

	m_currencyCounter = new Label("");

	m_currencyCounter->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.13f, m_screenWidth * 0.15f, m_screenHeight * 0.05f);
	m_currencyCounter->SetColour(255, 215, 0, 50);

	UpdateCurrency(0);

	//SET UP SELECTED UI
//	backBuffer.DrawRectangle(m_screenWidth * 0.76f, m_screenHeight * 0.52f, m_screenWidth * 0.99f, m_screenHeight * 0.92f, 1); // Background

	m_towerRange = new Button("");

	m_towerRange->SetBounds(m_screenWidth * 0.82f, m_screenHeight * 0.61f, m_screenWidth * 0.15f, m_screenHeight * 0.07f);
	m_towerRange->SetColour(178, 34, 34, 0);
	m_towerRange->SetFontSize(20);

	m_towerFireRate = new Button("");

	m_towerFireRate->SetBounds(m_screenWidth * 0.82f, m_screenHeight * 0.69f, m_screenWidth * 0.15f, m_screenHeight * 0.07f);
	m_towerFireRate->SetColour(178, 34, 34, 0);
	m_towerFireRate->SetFontSize(20);

	m_towerDamage = new Button("");

	m_towerDamage->SetBounds(m_screenWidth * 0.82f, m_screenHeight * 0.77f, m_screenWidth * 0.15f, m_screenHeight * 0.07f);
	m_towerDamage->SetColour(178, 34, 34, 0);
	m_towerDamage->SetFontSize(20);

	m_sell = new Button("");

	m_sell->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.87f, m_screenWidth * 0.23f, m_screenHeight * 0.05f);
	m_sell->SetColour(240, 230, 140, 0);
	m_sell->SetBackgroundColour(100, 100, 100);
	m_sell->SetFontSize(20);

	m_upgradeTower = new Button("");

	m_upgradeTower->SetBounds(m_screenWidth * 0.76f, m_screenHeight * 0.82f, m_screenWidth * 0.23f, m_screenHeight * 0.05f);
	m_upgradeTower->SetColour(0, 0, 0, 0);
	m_upgradeTower->SetFontSize(20);

	m_rangeSprite = m_pBackBuffer->CreateSprite("assets\\range_icon.png");
	m_rangeSprite->SetX(m_screenWidth * 0.76f);
	m_rangeSprite->SetY(m_screenHeight * 0.60f);

	m_speedSprite = m_pBackBuffer->CreateSprite("assets\\time_icon.png");
	m_speedSprite->SetX(m_screenWidth * 0.76f);
	m_speedSprite->SetY(m_screenHeight * 0.68f);

	m_damageSprite = m_pBackBuffer->CreateSprite("assets\\damage_icon.png");
	m_damageSprite->SetX(m_screenWidth * 0.76f);
	m_damageSprite->SetY(m_screenHeight * 0.76f);

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

	m_gameState = PLAYING;

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

	//Process projectiles
	std::vector<Projectile*>::iterator projectileIter = m_projectiles.begin();

	while (projectileIter != m_projectiles.end())
	{
		Projectile* proj = *projectileIter;

		proj->Process(deltaTime);

		std::vector<Entity*> entitiesInRange = m_quadTree->QueryRange(proj->GetCollisionBounds());

		if (!entitiesInRange.empty())
		{
			Enemy* hit = reinterpret_cast<Enemy*>(entitiesInRange[0]);
			hit->TakeDamage(proj->GetDamage());

			projectileIter = m_projectiles.erase(projectileIter);

			delete proj;
			proj = 0;
		}
		else if (proj->GetPositionX() < 0 || proj->GetPositionX() > m_screenHeight || proj->GetPositionY() < 0 || proj->GetPositionY() > m_screenHeight)
		{
			projectileIter = m_projectiles.erase(projectileIter);

			delete proj;
			proj = 0;
		}
		else
		{
			++projectileIter;
		}
	}

	ProcessEnemies(deltaTime);
	ProcessTowers(deltaTime);

	system->update(); // Update system
}

void Game::ProcessEnemies(float deltaTime)
{
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

			if (m_enemies.empty() && m_enemySpawner->WaveSpawned())
			{
				m_enemySpawner->EndWave();
				UpdateWaves();

				m_startWave->SetBackgroundColour(34, 139, 34);

				if (m_enemySpawner->GetWaveNumber() == m_enemySpawner->GetTotalWaveNumber())
				{
					m_gameState = WON;
				}
			}
		}
		else
		{
			current->Process(deltaTime);
			current->m_targetted = false;
			++enemyIter;
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
}

void Game::ProcessTowers(float deltaTime)
{
	//std::vector<Tower*>::iterator towerIter = m_towers.begin();

	//while (towerIter != m_towers.end())
	//{
	//	Tower* current = *towerIter;

	//	if (current->IsSold())
	//	{
	//		towerIter = m_towers.erase(towerIter);

	//		delete current;
	//		current = 0;
	//	}
	//	else
	//	{
	//		std::vector<Entity*> entitiesInRange = m_quadTree->QueryRange(current->GetRangeBounds());

	//		current->SetEnemiesInRange(entitiesInRange);

	//		for each (Enemy* enemy in entitiesInRange)
	//		{
	//			enemy->m_targetted = true;
	//		}

	//		current->Process(deltaTime);

	//		++towerIter;
	//	}
	//}

	for each (Tower* current in m_towers)
	{
		std::vector<Entity*> entitiesInRange = m_quadTree->QueryRange(current->GetRangeBounds());

		current->SetEnemiesInRange(entitiesInRange);

		for each (Enemy* enemy in entitiesInRange)
		{
			enemy->m_targetted = true;
		}

		current->Process(deltaTime);
	}
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

	for each (Projectile* projectile in m_projectiles)
	{
		projectile->Draw(backBuffer);
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

	//Draw building UI
	backBuffer.SetDrawColour(192, 192, 192);
	backBuffer.DrawRectangle(m_screenWidth * 0.76f, m_screenHeight * 0.20f, m_screenWidth * 0.99f, m_screenHeight * 0.5f, 1); // Background

	m_towerText->Draw(backBuffer); // Title

	//Buttons
	m_towerButton->Draw(backBuffer);
	m_wallButton->Draw(backBuffer);

	//Draw Selected UI
	DrawSelectionUI(backBuffer);

	//Start wave
	m_startWave->Draw(backBuffer);

	//Draw cursor
	if (m_cursorSprite != 0)
	{
		m_pBackBuffer->DrawSprite(*m_cursorSprite);
	}
}

void Game::DrawSelectionUI(BackBuffer& backBuffer)
{
	backBuffer.SetDrawColour(192, 192, 192);
	backBuffer.DrawRectangle(m_screenWidth * 0.76f, m_screenHeight * 0.52f, m_screenWidth * 0.99f, m_screenHeight * 0.92f, 1); // Background

	m_highlighted->Draw(backBuffer); // Selected title

	if (m_selectedTower != 0)
	{
		m_towerRange->Draw(backBuffer);
		m_towerFireRate->Draw(backBuffer);
		m_towerDamage->Draw(backBuffer);
		m_sell->Draw(backBuffer);
		m_upgradeTower->Draw(backBuffer);

		m_rangeSprite->Draw(backBuffer);
		m_damageSprite->Draw(backBuffer);
		m_speedSprite->Draw(backBuffer);

		if (m_currency >= m_selectedTower->GetTowerUpgradeCost())
		{
			m_upgradeTower->SetBackgroundColour(34, 139, 34);
		}
		else
		{
			m_upgradeTower->SetBackgroundColour(178, 34, 34);
		}
	}
}

void Game::UpdateLives(int amount)
{
	m_currentLives += amount;

	if (m_currentLives <= 0)
	{
		m_currentLives = 0;

		m_gameState = LOST;
	}

	std::stringstream lifeMessage;
	lifeMessage << "Lives: " << m_currentLives;

	m_lifeCounter->SetText(lifeMessage.str());
}

void Game::UpdateWaves()
{
	std::stringstream waveMessage;
	waveMessage << "Wave: " << m_enemySpawner->GetWaveNumber() << "/" << m_enemySpawner->GetTotalWaveNumber();
	m_waveCounter->SetText(waveMessage.str());
}

void Game::UpdateCurrency(int amount)
{
	m_currency += amount;

	std::stringstream currencyMessage;
	currencyMessage << "$" << m_currency;
	m_currencyCounter->SetText(currencyMessage.str());

	//Update buttons
	if (m_currency >= 10)
	{
		m_wallButton->SetBackgroundColour(34, 139, 34);
	}
	else
	{
		m_wallButton->SetBackgroundColour(178, 34, 34);
	}

	if (m_currency >= 100)
	{
		m_towerButton->SetBackgroundColour(34, 139, 34);
	}
	else
	{
		m_towerButton->SetBackgroundColour(178, 34, 34);
	}
}

void Game::UpdateSelected()
{
	if (m_selectedTower != 0)
	{
		std::stringstream message;
		message << "Range: " << m_selectedTower->GetTowerRange();

		m_towerRange->SetText(message.str());
		message.str("");

		message << std::setprecision(2);
		message << "Speed: " << 1.0f / m_selectedTower->GetTowerFireRate();

		m_towerFireRate->SetText(message.str());
		message.str("");

		message << "Damage: " << m_selectedTower->GetTowerDamage();

		m_towerDamage->SetText(message.str());
		message.str("");

		message << "Sell $" << m_selectedTower->GetTowerValue();

		m_sell->SetText(message.str());
		message.str("");
		
		if (m_selectedTower->IsMaxLevel())
		{
			message << "Maxed";
		}
		else
		{
			message << "Upgrade $" << m_selectedTower->GetTowerUpgradeCost();
		}

		m_upgradeTower->SetText(message.str());
		message.str("");
	}
	else
	{
		m_towerRange->SetText("");
		m_towerFireRate->SetText("");
		m_towerDamage->SetText("");
		m_sell->SetText("");

	}
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
	if (m_towerButton->WasClickedOn(x, y))
	{
		delete(m_cursorSprite);

		Sprite* newTowerSprite = m_pBackBuffer->CreateSprite("assets\\tower_base.png");
		m_cursorSprite = newTowerSprite;

		m_selected = TOWER;
	}
	else if (m_wallButton->WasClickedOn(x, y))
	{
		delete(m_cursorSprite);

		Sprite* wallSprite = m_pBackBuffer->CreateSprite("assets\\wall_base.png");

		m_cursorSprite = wallSprite;

		m_selected = WALL;
	}
	else if (m_startWave->WasClickedOn(x, y))
	{
		StartWave();
	}
	else if (m_sell->WasClickedOn(x, y))
	{
		if (m_selectedTower != 0)
		{
			SellTower(m_selectedTower);
			m_selectedTower = 0;
			UpdateSelected();
		}
	}
	else if (m_upgradeTower->WasClickedOn(x, y))
	{
		if (m_currency >= m_selectedTower->GetTowerUpgradeCost() && !m_selectedTower->IsMaxLevel())
		{
			UpdateCurrency(-m_selectedTower->GetTowerUpgradeCost());

			m_selectedTower->UpgradeTower();

			UpdateSelected();
		}
	}
	else
	{
		Position pos = Position(x, y);

		std::vector<Entity*> clickedOn = m_quadTree->QueryPoint(&pos);

		for each (Entity* e in clickedOn)
		{
			reinterpret_cast<Enemy*>(e)->TakeDamage(1);
		}

		if (m_selected == TOWER)
		{
			PlaceTower(x, y);
		}
		else if (m_selected == WALL)
		{
			PlaceWall(x, y);
		}
		else
		{
			Position pos = Position(x, y);

			std::vector<Entity*> clickedOn = m_towerQuadTree->QueryPoint(&pos);

			for each (Tower* t in clickedOn) // There should only ever be 1 tower in the vector
			{
				m_selectedTower = t;
				UpdateSelected();
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
	delete(m_cursorSprite);
	m_cursorSprite = 0;
	m_selected = NOTHING;
}

void Game::StartWave()
{
	if (m_enemySpawner->IsWaveActive()) // Cannot start a new wave while a wave is still active
	{
		return;
	}

	m_map->UpdatePath();

	std::queue<Position*> path = m_pathfinding->SimplifyPath(m_map->GetGridPath());

	m_enemySpawner->StartWave(path);

	delete(m_cursorSprite);
	m_cursorSprite = 0;
	m_selected = NOTHING;

	m_startWave->SetBackgroundColour(100, 100, 100);
}

void Game::AddEnemy(Enemy* enemy)
{
	enemy->SetTilePosition(m_map->GetGridStart());

	enemy->m_grid = m_map;

	m_quadTree->Insert(enemy);

	m_enemies.push_back(enemy);
}

void Game::AddProjectile(Projectile* projectile)
{
	m_projectiles.push_back(projectile);
}

void Game::PlaceTower(int x, int y)
{
	if (m_enemySpawner->IsWaveActive())
	{
		return;
	}

	Tile* currentTile = m_map->GetTileFromPixelCoord(x, y);

	if (m_currency < 100 || currentTile == 0)
	{
		return;
	}

	if (currentTile->GetState() == BLOCKED)
	{
		return;
	}

	currentTile->SetOccupied(true);

	if (!m_map->UpdatePath())
	{
		currentTile->SetOccupied(false);

		return;
	}

	Tower* newTower = new Tower(2, 0.5f, 1, 100);

	newTower->Initialise(m_pBackBuffer);

	//Scale to tile size
	newTower->GetSprite()->SetWidth(currentTile->GetTileWidth());
	newTower->GetSprite()->SetHeight(currentTile->GetTileHeight());

	newTower->SetTilePosition(currentTile);

	m_towers.push_back(newTower);

	m_map->UpdatePath();

	UpdateCurrency(-newTower->GetTowerCost());

	m_towerQuadTree->Insert(newTower);
}

void Game::SellTower(Tower* tower)
{
	UpdateCurrency(tower->GetTowerValue());

	tower->GetTilePosition()->SetOccupied(false);

	std::vector<Tower*>::iterator towerIter = m_towers.begin();

	while (towerIter != m_towers.end())
	{
		Tower* current = *towerIter;

		if (current == tower)
		{
			m_towers.erase(towerIter);
			break;
		}
		else
		{
			++towerIter;
		}
	}

	//Redo quad tree
	delete(m_towerQuadTree);

	Position* topLeftPos = new Position(m_screenHeight / 2, m_screenHeight / 2);
	AxisAlignedBoundingBox* gridBounds = new AxisAlignedBoundingBox(topLeftPos, m_screenHeight / 2);
	m_towerQuadTree = new QuadTree(gridBounds);

	for each (Tower* tower in m_towers)
	{
		m_towerQuadTree->Insert(tower);
	}

	m_map->UpdatePath();
}

void Game::PlaceWall(int x, int y)
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

void Game::UpdateCursorPosition(int x, int y)
{
	if (m_cursorSprite != 0)
	{
		m_cursorSprite->SetCenter(x, y);
	}
}