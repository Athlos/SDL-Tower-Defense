#ifndef __ENTITY_H__
#define __ENTITY_H__

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

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetDead(bool dead);
	bool IsDead() const;

	bool IsCollidingWith(int x, int y, int radius);

	float GetPositionX() const;
	float GetPositionY() const;

	float GetCenterX() const;
	float GetCenterY() const;

	float GetHorizontalVelocity();
	void SetHorizontalVelocity(float x); 

	float GetVerticalVelocity();
	void SetVerticalVelocity(float y);

	void SetPosition(float x, float y);
	bool IsClickedOn(int x, int y);

	Sprite* GetSprite();

	Position* GetPosition() const;

	AxisAlignedBoundingBox* GetCollisionBounds() const;

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	// Member Data:
protected:
	Sprite* m_pSprite;

	float m_x;
	float m_y;

	Position* m_pos;
	AxisAlignedBoundingBox* m_bounds;

	float m_velocityX;
	float m_velocityY;

	bool m_dead;
};

#endif //__ENTITY_H__
