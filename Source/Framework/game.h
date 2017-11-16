#ifndef __GAME_H__
#define __GAME_H__

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

#include "fmod.hpp";

#include <vector>;

enum BuildingTypeSelected
{
	WALL,
	TOWER,
	NOTHING
};

class Game
{
	// Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
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
	void PlaceWall(int x, int y);

	void UpdateCursorPosition(int x, int y);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void DrawUI(BackBuffer& backBuffer);
	void DrawSelectionUI(BackBuffer& backBuffer);

	void UpdateLives(int amount);
	void UpdateWaves();
	void UpdateCurrency(int amount);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);

	void TestQuadTree(int x, int y);
	
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
	BuildingTypeSelected m_selected;

	//GAME ENTITIES
	Grid* m_map; // Grid of map tiles
	Pathfinding* m_pathfinding; // Pathfinding class
	std::vector<Enemy*> m_enemies; // Container of enemies on map
	std::vector<Tower*> m_towers; // Container of towers in game
	std::vector<Projectile*> m_projectiles; // Container of projectiles in game TODO update to use a projectile pool of some sort

	EnemySpawner* m_enemySpawner; // Spawner for enemies

	ParticleEmitter* m_particles; // Particle emitter

	//WAVES
	int m_currentLives;
	int m_totalLives;

	//GAME COUNTERS
	int m_currency;

	//AUDIO
	FMOD::System *system;
	bool combatMusic;
	FMOD_RESULT result;
	FMOD::Channel* channel;
	FMOD::Channel* m_musicChannel;
};

#endif // __GAME_H__
