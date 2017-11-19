#pragma once

#include "entity.h"

#include <vector>
#include <queue>

class Tile;
class Grid;
class Position;
class AnimatedSprite;

class Enemy : public Entity
{
	// Member Methods:
public:
	Enemy();
	~Enemy();

	void SetPosition(float x, float y); // Set the position

	float GetCenterX() const; // Get the center of the enemy X position
	float GetCenterY() const;// Get the center of the enemy Y position

	bool Initialise(AnimatedSprite* sprite); // Initialise an enemy with a sprite

	void Process(float deltaTime); // Process the enemy
	void Draw(BackBuffer& backBuffer); // Draw the enemy to the screen

	void DrawHealthBar(BackBuffer& backBuffer); // Draw the health bar above the enemy

	void SetData(int health, float speed, int damage); // Set the core data for the enemy
	bool IsClickedOn(int x, int y); // Return if the enemy was clicked on

	int GetDamage(); // Get the amount of damage the enemy does
	void TakeDamage(int amount); // Reduce the enemy health by a certain amount

	int GetReward(); // Get the currency reward value from killing the enemy
	void SetReward(int amount); // Set the currency reward value from killing the enemy

	void SetTilePosition(Tile* tile); // Set the position on map based on tile
	Tile* GetTilePosition(); // Get the position of the tile

	void SetPath(std::queue<Position*> path); // Set the path for the enemy to follow

	void MoveToWaypoints(float deltaTime); // Moves towards the current waypoint

	bool ReachedEnd() const; // Returns if the enemy has reached the end

	int WaypointsToGo() const; // Returns how many waypoints are left to go through
	float DistanceToNextWaypoint() const; // Get distance to next waypoint

private:
	void UpdateDirection(); // Updates the direction the enemy should move to

	// Member Data:
public:
	Grid* m_grid; // Map grid

	bool m_inRange; // Flag if enemy is in range of a tower
	bool m_targetted; // Flag if enemy is targetted by a tower

protected:
	int m_currentHealth; // Enemy current health
	int m_maxHealth; // Enemy max health

	int m_damage; // Damage the enemy does
	int m_reward; // Reward from killing enemy

	float m_speed; // Enemy movement speed
	float m_directionX; // Enemy x direction
	float m_directionY; // Enemy y direction

	Tile* m_tilePosition; // Enemy tile location on map

	std::queue<Position*> m_waypoints; // Waypoints to follow to reach end of map
	Position* m_currentWaypoint; // Current waypoint to move towards

	bool m_reachedEnd; // Flag if enemy has reached final waypoint
	float m_distanceToNextWaypoint; // Distance to current waypoint

	AnimatedSprite* m_animSprite; // Enemy sprite
};