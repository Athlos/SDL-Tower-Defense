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
class InterfaceManager;
class SniperTower;
class PulseTower;

#include "fmod.hpp";
#include "building.h"

#include <vector>;

enum GameState // State of the game
{
	PAUSED,
	PLAYING,
	WON,
	LOST
};

enum SelectedToBuild
{
	BUILD_NONE,
	BUILD_WALL,
	BUILD_SNIPER,
	BUILD_PULSE
};

class Game
{
	// Member Methods:
public:
	static Game& GetInstance(); // Get singleton instance
	static void DestroyInstance(); // Destroy singleton instance
	~Game(); // Destroy game

	bool Initialise(bool firstTime); // Initialise game elements

	bool DoGameLoop(); // Run game loop
	void Quit(); // Quit game

	void Pause(bool pause); // Set if game is paused
	bool IsPaused(); // Returns if game is paused

	void OnLeftMouseClick(int x, int y); // Processes what to do on a left mouse click
	void OnRightMouseClick(int x, int y); // Processes what to do on a right mouse click

	void StartWave(); // Starts the next enemy wave
	void AddEnemy(Enemy* enemy); // Adds an enemy to the game, it will appear at the map start
	void AddProjectile(Projectile* projectile); // Adds a projectile to the game

	void UpdateCursorPosition(int x, int y); // Keeps track of cursor position
	void UpdateGameState(GameState state); // Set the game state

	void PlaySound(const char* filename); // Play a sound through the audio manager
	
protected:
	void CleanUp(); // delete game elements for restarting

	void Process(float deltaTime); // Process over game elements
	void ProcessEnemies(float deltaTime); // Process over enemy container
	void ProcessTowers(float deltaTime); // Process over tower container
	void ProcessProjectiles(float deltaTime); // Process over projectile container

	void Draw(BackBuffer& backBuffer); // Draw game elements
	void DrawUI(BackBuffer& backBuffer); // Draw UI elements
	void DrawSelectionUI(BackBuffer& backBuffer); // Draw Selection elements
	void DrawEndGameUI(BackBuffer& backBuffer); // Draw game paused menu

	void UpdateLives(int amount); // Update lives counter and text
	void UpdateWaves(); // Update wave counter and text
	void UpdateCurrency(int amount); // Update currency counter and text
	void UpdateSelected(); // Update selection UI

	void PlaceTower(int x, int y); // Places a tower on the map
	void SellBuilding(Building* building); // Sells a building
	void PlaceWall(int x, int y); // Places a wall on the map

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	// Member Data:
public:

	const static int SCREEN_WIDTH = 800; // Screen width
	const static int SCREEN_HEIGHT = 600; // Screen height

	QuadTree* m_quadTree;

protected:
	static Game* sm_pInstance; // Game singleton instance
	BackBuffer* m_pBackBuffer; // Backbuffer that renders game elements
	InputHandler* m_pInputHandler; // Input handler
	bool m_looping; // Flag for game loop

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

	GameState m_gameState; // State of game for pause menu

	//UI ELEMENTS
	InterfaceManager* m_interfaceManager; // Holds all the buttons, icons and labels

	Sprite* m_cursorSprite; // Sprite attached to cursor to show which building will be placed
	SelectedToBuild m_selected; // tag to choose which sprite will be the cursor sprite

	//GAME ENTITIES
	Grid* m_map; // Grid of map tiles
	Pathfinding* m_pathfinding; // Pathfinding class
	std::vector<Enemy*> m_enemies; // Container of enemies on map
	std::vector<Building*> m_buildings; // Container of towers in game
	std::vector<Projectile*> m_projectiles; // Container of projectiles in game TODO update to use a projectile pool of some sort

	EnemySpawner* m_enemySpawner; // Spawner for enemies

	ParticleEmitter* m_particles; // Particle emitter

	QuadTree* m_towerQuadTree; // Quad tree to store all buildings that are clicked on
	Building* m_selectedBuilding; // Building that is currently selected

	//WAVES
	int m_currentLives; // Current amount of lives
	int m_totalLives; // Max lives

	//GAME COUNTERS
	int m_currency; // Amount of currency

	//AUDIO
	AudioManager* m_audioManager; // Holds all the audio and can play them based on a string
};
