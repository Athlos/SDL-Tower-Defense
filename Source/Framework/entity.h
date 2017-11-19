#pragma once

// Forward Declarations:
class BackBuffer;
class Sprite;
class Position;
class AxisAlignedBoundingBox;

class Entity
{
	// Member Methods:
public:
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);

	void Process(float deltaTime); // Process the entity
	void Draw(BackBuffer& backBuffer); // Draw th entity

	void SetDead(bool dead); // Set the enemy alive status
	bool IsDead() const; // Get if the enemy is dead

	bool IsCollidingWith(int x, int y, int radius); // Return if the enemy is colliding with a specific radius, currently unecessary with quad tree
	bool IsClickedOn(int x, int y); // Return if the enemy was clicked on

	float GetPositionX() const; // Get x position
	float GetPositionY() const; // Get y position

	float GetCenterX() const; // Get x center
	float GetCenterY() const; // Get y center

	float GetHorizontalVelocity(); // Get horizontal velocity
	void SetHorizontalVelocity(float x); // Set the horizontal velocity

	float GetVerticalVelocity(); // Get vertical velocity
	void SetVerticalVelocity(float y); // Set vertical velocity

	Sprite* GetSprite(); // Get the entity sprite

	Position* GetPosition() const; // Gets the current position as a position class
	void SetPosition(float x, float y); // Sets the position

	AxisAlignedBoundingBox* GetCollisionBounds() const; // Get the collision bounds
	void SetCollisionBounds(Position* center, float radius); // Set the collision bounds

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	// Member Data:
protected:
	Sprite* m_pSprite; // The entity sprite

	float m_x; // X position
	float m_y; // Y position

	Position* m_pos; // Position struct, currently duplicates with mx and my due to compatibility with the old system, will replace those 
	AxisAlignedBoundingBox* m_bounds; // Collision bounds for the quadtree

	float m_velocityX; // X velocity
	float m_velocityY; // Y velocity

	bool m_dead; // flag for enemy alive status
};
