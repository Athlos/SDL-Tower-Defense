#pragma once

// Forward Declarations:
class BackBuffer;
class Texture;

class Sprite
{
	// Member Methods:
public:
	Sprite();
	~Sprite();

	bool Initialise(Texture& texture); // Create a sprite using a texture

	void Process(float deltaTime); // Process over the sprite
	void Draw(BackBuffer& backbuffer); // Draw the sprite

	void SetX(int x); // Set X position
	void SetY(int y); // Set Y position

	int GetX() const; // Get X position
	int GetY() const; // Get Y position

	int GetCenterX() const; // Get X center
	int GetCenterY() const; // Get Y center

	void SetAngle(float angle); // Set the sprite angle
	float GetAngle() const; // Get the angle

	void SetCenter(int x, int y); // Set the center of the sprite

	int GetWidth() const; // Get the width of the sprite
	int GetHeight() const; // Get the height of the sprite

	void SetWidth(int width); // Set the sprite width
	void SetHeight(int height); // Set sprite height

	Texture* GetTexture(); // Get the texture

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	// Member Data:
protected:
	Texture* m_pTexture; // Sprite texture
	int m_x; // X position
	int m_y; // Y position

	float m_angle; // Sprite angle
	int m_centerX; // Center X position
	int m_centerY; // Center Y position

	int m_width; // Width of the sprite
	int m_height; // Height of the sprite

};