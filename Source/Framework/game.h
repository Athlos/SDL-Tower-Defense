#pragma once

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class Label;
class Button;
class ParticleEmitter;
class Grid;
class Pathfinding;
class Enemy;
class Tower;
class Position;
class QuadTree;
class EnemySpawner;
class Projectile;
class AudioManager;
class Wall;

#include "fmod.hpp";
#include "building.h"

#include <vector>;

enum GameState
{
	PAUSED,
	PLAYING,
	WON,
	LOST
};

class Game
{
	// Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise(bool firstTime);
	void CleanUp();

	bool DoGameLoop();
	void Quit();

	void Pause(bool pause);
	bool IsPaused();

	void OnLeftMouseClick(int x, int y);
	void OnRightMouseClick(int x, int y);

	void StartWave();
	void AddEnemy(Enemy* enemy);
	void AddProjectile(Projectile* projectile);

	void PlaceTower(int x, int y);
	void SellBuilding(Building* building);
	void PlaceWall(int x, int y);

	void UpdateCursorPosition(int x, int y);
	void UpdateGameState(GameState state);
	
protected:
	void Process(float deltaTime);
	void ProcessEnemies(float deltaTime);
	void ProcessTowers(float deltaTime);
	void ProcessProjectiles(float deltaTime);

	void Draw(BackBuffer& backBuffer);

	void DrawUI(BackBuffer& backBuffer);
	void DrawSelectionUI(BackBuffer& backBuffer);
	void DrawEndGameUI(BackBuffer& backBuffer);

	void UpdateLives(int amount);
	void UpdateWaves();
	void UpdateCurrency(int amount);
	void UpdateSelected();

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	// Member Data:
public:

	const static int m_screenWidth = 800;
	const static int m_screenHeight = 600;

	QuadTree* m_quadTree;

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;
	
	bool m_paused;

	GameState m_gameState;

	//UI ELEMENTS
	Label* m_debug_fps; // FPS counter
	Label* m_lifeCounter; // Life counter
	Label* m_waveCounter; // Current wave counter
	Label* m_currencyCounter; // Currency amount counter

	Label* m_towerText;
	Label* m_highlighted;
	
	Button* m_wallButton;
	Button* m_towerButton;
	Button* m_startWave;

	Sprite* m_cursorSprite;
	BuildingType m_selected;

	Button* m_towerRange;
	Sprite* m_rangeSprite;

	Button* m_towerFireRate;
	Sprite* m_speedSprite;

	Button* m_towerDamage;
	Sprite* m_damageSprite;

	Button* m_sell;
	Button* m_upgradeTower;

	//end game ui
	Label* m_gameOver;
	Button* m_quit;
	Button* m_restart;

	//GAME ENTITIES
	Grid* m_map; // Grid of map tiles
	Pathfinding* m_pathfinding; // Pathfinding class
	std::vector<Enemy*> m_enemies; // Container of enemies on map
	std::vector<Building*> m_buildings; // Container of towers in game
	std::vector<Projectile*> m_projectiles; // Container of projectiles in game TODO update to use a projectile pool of some sort

	EnemySpawner* m_enemySpawner; // Spawner for enemies

	ParticleEmitter* m_particles; // Particle emitter

	QuadTree* m_towerQuadTree;
	Building* m_selectedBuilding;

	//WAVES
	int m_currentLives;
	int m_totalLives;

	//GAME COUNTERS
	int m_currency;

	//AUDIO
	AudioManager* m_audioManager;
};
