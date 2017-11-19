// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
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
#include "audiomanager.h"
#include "interfacemanager.h"
#include "wall.h"

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
, m_pathfinding(0)
{
	// Seed rand
	srand(time(0));
}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_particles;
	m_particles = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_audioManager;
	m_audioManager = 0;

	CleanUp();
}

/*
Initialises renderer, input, audio, player and other variables needed to begin
*/
bool Game::Initialise(bool firstTime)
{
	if (firstTime)
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

		//Create UI
		m_interfaceManager = new InterfaceManager(m_pBackBuffer);

		//UI counters
		m_interfaceManager->AddLabel("fpsCounter", "", m_screenWidth - 48, 0, 48, 24, GREEN);
		m_interfaceManager->AddLabel("lifeCounter", "", m_screenWidth * 0.76f, m_screenHeight * 0.01f, m_screenWidth * 0.15f, m_screenHeight * 0.05f, RED);
		m_interfaceManager->AddLabel("waveCounter", "", m_screenWidth * 0.76f, m_screenHeight * 0.07f, m_screenWidth * 0.15f, m_screenHeight * 0.05f, BLUE);
		m_interfaceManager->AddLabel("currencyCounter", "", m_screenWidth * 0.76f, m_screenHeight * 0.13f, m_screenWidth * 0.15f, m_screenHeight * 0.05f, GOLD);

		m_interfaceManager->GetLabel("fpsCounter")->SetDrawable(false);

		//UI static text
		m_interfaceManager->AddLabel("buildingText", "Buildings", m_screenWidth * 0.76f, m_screenHeight * 0.20f, m_screenWidth * 0.23f, m_screenHeight * 0.07f, RED);
		m_interfaceManager->AddLabel("selected", "Selected", m_screenWidth * 0.76f, m_screenHeight * 0.52f, m_screenWidth * 0.23f, m_screenHeight * 0.07f, RED);
		m_interfaceManager->AddLabel("gameOver", "", m_screenWidth * 0.3f, m_screenHeight * 0.3f, m_screenWidth * 0.4f, m_screenHeight * 0.1f, BLACK);

		m_interfaceManager->GetLabel("buildingText")->SetTextAlignment(CENTER);
		m_interfaceManager->GetLabel("selected")->SetTextAlignment(CENTER);
		m_interfaceManager->GetLabel("gameOver")->SetTextAlignment(CENTER);
		m_interfaceManager->GetLabel("gameOver")->SetDrawable(false);

		//UI selection text
		m_interfaceManager->AddLabel("towerRangeText", "", m_screenWidth * 0.82f, m_screenHeight * 0.61f, m_screenWidth * 0.15f, m_screenHeight * 0.07f, DARKRED);
		m_interfaceManager->AddLabel("towerDamageText", "", m_screenWidth * 0.82f, m_screenHeight * 0.77f, m_screenWidth * 0.15f, m_screenHeight * 0.07f, DARKRED);
		m_interfaceManager->AddLabel("towerSpeedText", "", m_screenWidth * 0.82f, m_screenHeight * 0.69f, m_screenWidth * 0.15f, m_screenHeight * 0.07f, DARKRED);

		//UI selection buttons
		m_interfaceManager->AddButton("wallBuildButton", "", "assets\\wall_base.png", m_screenWidth * 0.77f, m_screenHeight * 0.28f, m_screenWidth * 0.05f, m_screenWidth * 0.05f, BLACK);
		m_interfaceManager->AddButton("towerBuildButton", "", "assets\\tower_base.png", m_screenWidth * 0.84f, m_screenHeight * 0.28f, m_screenWidth * 0.05f, m_screenWidth * 0.05f, BLACK);
		m_interfaceManager->AddButton("upgradeButton", "", "", m_screenWidth * 0.76f, m_screenHeight * 0.82f, m_screenWidth * 0.23f, m_screenHeight * 0.05f, BLACK);
		m_interfaceManager->AddButton("sellButton", "", "", m_screenWidth * 0.76f, m_screenHeight * 0.87f, m_screenWidth * 0.23f, m_screenHeight * 0.05f, GOLD);

		m_interfaceManager->GetButton("upgradeButton")->SetDrawable(false);

		//UI selection icons
		m_interfaceManager->AddIcon("rangeIcon", "assets\\range_icon.png", m_screenWidth * 0.76f, m_screenHeight * 0.60f, 32, 32);
		m_interfaceManager->AddIcon("speedIcon", "assets\\time_icon.png", m_screenWidth * 0.76f, m_screenHeight * 0.68f, 32, 32);
		m_interfaceManager->AddIcon("damageIcon", "assets\\damage_icon.png", m_screenWidth * 0.76f, m_screenHeight * 0.76f, 32, 32);

		//UI start wave
		m_interfaceManager->AddButton("startWaveButton", "Start Wave", "", m_screenWidth * 0.76f, m_screenHeight * 0.93f, m_screenWidth * 0.23f, m_screenWidth * 0.05f, BLACK);
		m_interfaceManager->GetButton("startWaveButton")->SetTextAlignment(CENTER);

		//UI end game buttons
		m_interfaceManager->AddButton("quitButton", "Quit", "", m_screenWidth * 0.3f, m_screenHeight * 0.6f, m_screenWidth * 0.2f, m_screenHeight * 0.1f, BLACK);
		m_interfaceManager->AddButton("restartButton", "Restart", "", m_screenWidth * 0.5f, m_screenHeight * 0.6f, m_screenWidth * 0.2f, m_screenHeight * 0.1f, BLACK);

		m_interfaceManager->GetButton("quitButton")->SetTextAlignment(CENTER);
		m_interfaceManager->GetButton("restartButton")->SetTextAlignment(CENTER);

		//Set up data
		m_pathfinding = new Pathfinding();

		//SET UP AUDIO
		m_audioManager = new AudioManager();
	}

	//SET UP ENTITIES AND VARIABLES
	m_map = new Grid(15, 15);

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

	m_totalLives = 10;
	m_currentLives = m_totalLives;

	m_currency = 500;

	m_enemySpawner = new EnemySpawner(0.5f, m_pBackBuffer);

	m_selectedBuilding = 0;

	//SET UP UI
	UpdateLives(0);
	UpdateCurrency(0);
	UpdateWaves();

	m_selected = NONE;

	UpdateSelected();

	m_cursorSprite = 0;

	m_interfaceManager->GetButton("startWaveButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKGREEN));

	// Timings
	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	UpdateGameState(PLAYING);

	return (true);
}

void Game::CleanUp()
{
	//Clear out containers
	for each (Enemy* enemy in m_enemies)
	{
		delete enemy;
		enemy = 0;
	}

	m_enemies.clear();

	for each (Building* tower in m_buildings)
	{
		delete tower;
		tower = 0;
	}

	m_buildings.clear();

	for each (Projectile* projectile in m_projectiles)
	{
		delete projectile;
		projectile = 0;
	}

	m_projectiles.clear();

	delete m_map;
	m_map = 0;

	delete m_quadTree;
	m_quadTree = 0;

	delete m_towerQuadTree;
	m_towerQuadTree = 0;

	delete m_particles;
	m_particles = 0;

	delete m_enemySpawner;
	m_enemySpawner = 0;
}

bool Game::DoGameLoop()
{
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
		m_interfaceManager->GetLabel("fpsCounter")->SetText(fpsString);
	}

	// Check if game is paused
	if (m_paused)
	{
		return;
	}

	m_particles->Process(deltaTime); // Process particles

	if (m_gameState == PLAYING)
	{
		m_enemySpawner->Process(deltaTime);

		ProcessProjectiles(deltaTime);
		ProcessEnemies(deltaTime);
		ProcessTowers(deltaTime);
	}

	m_audioManager->Process(deltaTime);
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

				m_interfaceManager->GetButton("startWaveButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKGREEN));

				if (m_enemySpawner->AllWavesCompleted())
				{
					UpdateGameState(WON);
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
	for each (Building* current in m_buildings)
	{
		if (current->GetType() == TOWER)
		{
			std::vector<Entity*> entitiesInRange = m_quadTree->QueryRange(dynamic_cast<Tower*>(current)->GetRangeBounds());

			dynamic_cast<Tower*>(current)->SetEnemiesInRange(entitiesInRange);

			for each (Enemy* enemy in entitiesInRange)
			{
				enemy->m_targetted = true;
			}

			dynamic_cast<Tower*>(current)->Process(deltaTime);
		}
		else if (current->GetType() == WALL)
		{
			//Nothing to process
			dynamic_cast<Wall*>(current)->Process(deltaTime);
		}
	}
}

void Game::ProcessProjectiles(float deltaTime)
{
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

			m_audioManager->PlaySound("assets\\audio\\enemy_end.wav");
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

	for each (Building* current in m_buildings)
	{
		if (current->GetType() == TOWER)
		{
			dynamic_cast<Tower*>(current)->Draw(backBuffer);
		}
		else if (current->GetType() == WALL)
		{
			//Nothing to process
			dynamic_cast<Wall*>(current)->Draw(backBuffer);
		}
	}

	for each (Projectile* projectile in m_projectiles)
	{
		projectile->Draw(backBuffer);
	}

	DrawUI(backBuffer);

	m_interfaceManager->Draw();

	//m_quadTree->Draw(backBuffer);

	backBuffer.Present();
}

void Game::DrawUI(BackBuffer& backBuffer)
{
	//Draw UI

	//Draw building UI
	backBuffer.SetDrawColour(192, 192, 192);
	backBuffer.DrawRectangle(m_screenWidth * 0.76f, m_screenHeight * 0.20f, m_screenWidth * 0.99f, m_screenHeight * 0.5f, 1); // Background

	//Draw Selected UI
	DrawSelectionUI(backBuffer);


	//Draw cursor
	if (m_cursorSprite != 0)
	{
		m_pBackBuffer->DrawSprite(*m_cursorSprite);
	}

	//Draw End Game UI if game is over
	if (m_gameState != PLAYING)
	{
		DrawEndGameUI(backBuffer);
	}
}

void Game::DrawSelectionUI(BackBuffer& backBuffer)
{
	backBuffer.SetDrawColour(192, 192, 192);
	backBuffer.DrawRectangle(m_screenWidth * 0.76f, m_screenHeight * 0.52f, m_screenWidth * 0.99f, m_screenHeight * 0.92f, 1); // Background

	if (m_selectedBuilding != 0)
	{
		if (m_selectedBuilding->GetType() == TOWER)
		{
			if (m_currency >= dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerUpgradeCost() && !dynamic_cast<Tower*>(m_selectedBuilding)->IsMaxLevel())
			{
				m_interfaceManager->GetButton("upgradeButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKGREEN));
			}
			else
			{
				m_interfaceManager->GetButton("upgradeButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKRED));
			}
		}
	}
}

void Game::DrawEndGameUI(BackBuffer & backBuffer)
{
	backBuffer.SetDrawColour(0, 0, 0);
	backBuffer.DrawRectangle(m_screenWidth * 0.29f, m_screenHeight * 0.29f, m_screenWidth * 0.71, m_screenHeight * 0.71f, 1); // Frame

	backBuffer.SetDrawColour(192, 192, 192);
	backBuffer.DrawRectangle(m_screenWidth * 0.3f, m_screenHeight * 0.30f, m_screenWidth * 0.7f, m_screenHeight * 0.7f, 1); // Background
}

void Game::UpdateLives(int amount)
{
	m_currentLives += amount;

	if (m_currentLives <= 0)
	{
		m_currentLives = 0;

		UpdateGameState(LOST);
	}

	std::stringstream lifeMessage;
	lifeMessage << "Lives: " << m_currentLives;

	m_interfaceManager->GetLabel("lifeCounter")->SetText(lifeMessage.str());
}

void Game::UpdateWaves()
{
	std::stringstream waveMessage;
	waveMessage << "Wave: " << m_enemySpawner->GetWaveNumber() << "/" << m_enemySpawner->GetTotalWaveNumber();
	m_interfaceManager->GetLabel("waveCounter")->SetText(waveMessage.str());
}

void Game::UpdateCurrency(int amount)
{
	m_currency += amount;

	std::stringstream currencyMessage;
	currencyMessage << "$" << m_currency;
	m_interfaceManager->GetLabel("currencyCounter")->SetText(currencyMessage.str());

	//Update buttons
	if (m_currency >= 10)
	{
		m_interfaceManager->GetButton("wallBuildButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKGREEN));
	}
	else
	{
		m_interfaceManager->GetButton("wallBuildButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKRED));
	}

	if (m_currency >= 100)
	{
		m_interfaceManager->GetButton("towerBuildButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKGREEN));
	}
	else
	{
		m_interfaceManager->GetButton("towerBuildButton")->SetBackgroundColour(m_interfaceManager->GetColour(DARKRED));
	}
}

void Game::UpdateSelected()
{
	bool draw = false;

	if (m_selectedBuilding != 0)
	{
		std::stringstream message;

		if (m_selectedBuilding->GetType() == TOWER)
		{
			draw = true;

			message << "Range: " << dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerRange();

			m_interfaceManager->GetLabel("towerRangeText")->SetText(message.str());
			message.str("");

			message << std::setprecision(2);
			message << "Speed: " << 1.0f / dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerFireRate();

			m_interfaceManager->GetLabel("towerSpeedText")->SetText(message.str());
			message.str("");

			message << "Damage: " << dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerDamage();

			m_interfaceManager->GetLabel("towerDamageText")->SetText(message.str());
			message.str("");

			message << "Sell $" << dynamic_cast<Tower*>(m_selectedBuilding)->GetSellValue();

			m_interfaceManager->GetButton("sellButton")->SetText(message.str());
			message.str("");

			if (dynamic_cast<Tower*>(m_selectedBuilding)->IsMaxLevel())
			{
				message << "Maxed";
			}
			else
			{
				message << "Upgrade $" << dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerUpgradeCost();
			}

			m_interfaceManager->GetButton("upgradeButton")->SetText(message.str());
			message.str("");

			message << "Level " << dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerLevel() << " tower";

			m_interfaceManager->GetLabel("selected")->SetText(message.str());

			m_interfaceManager->GetButton("sellButton")->SetDrawable(true);
		}
		else if (m_selectedBuilding->GetType() == WALL)
		{
			m_interfaceManager->GetButton("sellButton")->SetDrawable(true);

			draw = false;

			m_interfaceManager->GetLabel("selected")->SetText("Wall");

			message << "Sell $" << dynamic_cast<Wall*>(m_selectedBuilding)->GetSellValue();

			m_interfaceManager->GetButton("sellButton")->SetText(message.str());
			message.str("");
		}
	}
	else
	{
		m_interfaceManager->GetLabel("selected")->SetText("Selected");

		m_interfaceManager->GetButton("sellButton")->SetDrawable(false);

		draw = false;

		m_interfaceManager->GetButton("sellButton")->SetText("");
	}

	m_interfaceManager->GetLabel("towerRangeText")->SetDrawable(draw);
	m_interfaceManager->GetLabel("towerSpeedText")->SetDrawable(draw);
	m_interfaceManager->GetLabel("towerDamageText")->SetDrawable(draw);
	m_interfaceManager->GetButton("upgradeButton")->SetDrawable(draw);

	m_interfaceManager->GetIcon("rangeIcon")->drawable = draw;
	m_interfaceManager->GetIcon("speedIcon")->drawable = draw;
	m_interfaceManager->GetIcon("damageIcon")->drawable = draw;
}

void Game::Quit()
{
	m_looping = false;
}

void Game::Pause(bool pause)
{
	m_paused = pause;
}

bool Game::IsPaused()
{
	return (m_paused);
}

void Game::OnLeftMouseClick(int x, int y)
{
	if (m_interfaceManager->GetButton("towerBuildButton")->WasClickedOn(x, y))
	{
		delete(m_cursorSprite);

		Sprite* newTowerSprite = m_pBackBuffer->CreateSprite("assets\\tower_base.png");
		m_cursorSprite = newTowerSprite;

		m_selected = TOWER;
	}
	else if (m_interfaceManager->GetButton("wallBuildButton")->WasClickedOn(x, y))
	{
		delete(m_cursorSprite);

		Sprite* wallSprite = m_pBackBuffer->CreateSprite("assets\\wall_base.png");

		m_cursorSprite = wallSprite;

		m_selected = WALL;
	}
	else if (m_interfaceManager->GetButton("startWaveButton")->WasClickedOn(x, y))
	{
		StartWave();
	}
	else if (m_interfaceManager->GetButton("sellButton")->WasClickedOn(x, y))
	{
		if (m_selectedBuilding != 0)
		{
			SellBuilding(m_selectedBuilding);

			m_selectedBuilding = 0;
			UpdateSelected();
		}
	}
	else if (m_interfaceManager->GetButton("upgradeButton")->WasClickedOn(x, y))
	{
		if (m_currency >= dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerUpgradeCost() && !dynamic_cast<Tower*>(m_selectedBuilding)->IsMaxLevel())
		{
			UpdateCurrency(-dynamic_cast<Tower*>(m_selectedBuilding)->GetTowerUpgradeCost());

			dynamic_cast<Tower*>(m_selectedBuilding)->UpgradeTower();

			UpdateSelected();

			m_audioManager->PlaySound("assets\\audio\\tower_place.wav");
		}
	}
	else if (m_interfaceManager->GetButton("quitButton")->WasClickedOn(x, y) && m_gameState != PLAYING)
	{
		Quit();
	}
	else if (m_interfaceManager->GetButton("restartButton")->WasClickedOn(x, y) && m_gameState != PLAYING)
	{
		CleanUp();
		Initialise(false);
	}
	else
	{
		Position pos = Position(x, y);

		std::vector<Entity*> clickedOn = m_quadTree->QueryPoint(&pos);

		for each (Entity* e in clickedOn)
		{
			reinterpret_cast<Enemy*>(e)->TakeDamage(1);

			m_audioManager->PlaySound("assets\\audio\\enemy_end.wav");
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
			m_selected = NONE;
			Position pos = Position(x, y);

			std::vector<Entity*> clickedOn = m_towerQuadTree->QueryPoint(&pos);

			if (clickedOn.empty())
			{
				if (m_selectedBuilding != 0)
				{
					m_selectedBuilding->SetSelected(false);
				}

				m_selectedBuilding = 0;
				UpdateSelected();
			}
			else
			{
				for each (Building* building in clickedOn) // There should only ever be 1 tower in the vector
				{
					if (m_selectedBuilding != 0)
					{
						m_selectedBuilding->SetSelected(false);
					}

					m_selectedBuilding = building;

					m_selectedBuilding->SetSelected(true);

					UpdateSelected();
				}
			}
		}
	}
	
}

void Game::OnRightMouseClick(int x, int y)
{
	delete(m_cursorSprite);
	m_cursorSprite = 0;
	m_selected = NONE;
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
	m_selected = NONE;

	m_interfaceManager->GetButton("startWaveButton")->SetBackgroundColour(m_interfaceManager->GetColour(GRAY));

	m_audioManager->PlaySound("assets\\audio\\wave_start.wav");
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
	m_audioManager->PlaySound("assets\\audio\\tower_shoot.wav");
	m_projectiles.push_back(projectile);
}

void Game::PlaceTower(int x, int y)
{
	if (m_enemySpawner->IsWaveActive())
	{
		return;
	}

	Tile* currentTile = m_map->GetTileFromPixelCoord(x, y);

	if (m_currency < 100)
	{
		m_audioManager->PlaySound("assets\\audio\\error.wav");
		return;
	}

	if (currentTile == 0)
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

	newTower->SetTilePosition(currentTile);

	m_buildings.push_back(newTower);

	m_map->UpdatePath();

	UpdateCurrency(-newTower->GetCost());

	m_towerQuadTree->Insert(newTower);

	m_audioManager->PlaySound("assets\\audio\\tower_place.wav");
}

void Game::SellBuilding(Building* building)
{
	if (building->GetType() == TOWER)
	{
		UpdateCurrency(dynamic_cast<Tower*>(building)->GetSellValue());
	}
	else if (m_selectedBuilding->GetType() == WALL)
	{
		UpdateCurrency(dynamic_cast<Wall*>(building)->GetSellValue());
	}

	m_audioManager->PlaySound("assets\\audio\\coin.wav");

	building->GetTilePosition()->SetOccupied(false);

	std::vector<Building*>::iterator towerIter = m_buildings.begin();

	while (towerIter != m_buildings.end())
	{
		Building* current = *towerIter;

		if (current == building)
		{
			m_buildings.erase(towerIter);
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

	for each (Building* tower in m_buildings)
	{
		m_towerQuadTree->Insert(tower);
	}

	m_map->UpdatePath();
}

void Game::PlaceWall(int x, int y)
{
	if (m_enemySpawner->IsWaveActive()) 
	{
		return;
	}
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

				Wall* newWall = new Wall(10);

				newWall->Initialise(m_pBackBuffer);

				newWall->SetTilePosition(clicked);

				m_buildings.push_back(newWall);

				m_towerQuadTree->Insert(newWall);

				m_audioManager->PlaySound("assets\\audio\\wall_place.wav");
			}
		}
		else if(m_currency < 10)
		{
			m_audioManager->PlaySound("assets\\audio\\error.wav");
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

void Game::UpdateGameState(GameState state)
{
	if (m_gameState == PAUSED && state == PAUSED)
	{
		state = PLAYING;
	}

	m_gameState = state;

	if (m_gameState == PLAYING)
	{
		m_interfaceManager->GetLabel("gameOver")->SetDrawable(false);
		m_interfaceManager->GetButton("quitButton")->SetDrawable(false);
		m_interfaceManager->GetButton("restartButton")->SetDrawable(false);
	}
	else
	{
		m_interfaceManager->GetLabel("gameOver")->SetDrawable(true);
		m_interfaceManager->GetButton("quitButton")->SetDrawable(true);
		m_interfaceManager->GetButton("restartButton")->SetDrawable(true);
	}

	switch (m_gameState)
	{
	case WON:
	{
		m_interfaceManager->GetLabel("gameOver")->SetText("You Win");
	}
	break;
	case LOST:
	{
		m_interfaceManager->GetLabel("gameOver")->SetText("You Lost");
	}
	break;
	default:
	{
		m_interfaceManager->GetLabel("gameOver")->SetText("Paused");
	}
	break;
	}
}