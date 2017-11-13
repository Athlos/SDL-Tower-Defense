#ifndef __SPRITE_H__
#define __SPRITE_H__

// Forward Declarations:
class BackBuffer;
class Texture;

class Sprite
{
	// Member Methods:
public:
	Sprite();
	~Sprite();

	bool Initialise(Texture& texture);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetX(int x);
	void SetY(int y);

	int GetX() const;
	int GetY() const;

	int GetCenterX() const;
	int GetCenterY() const;

	void SetAngle(float angle);
	float GetAngle() const;

	void SetCenter(int x, int y);
	void SetHandleCenter();

	int GetWidth() const;
	int GetHeight() const;

	void SetWidth(int width);
	void SetHeight(int height);

	Texture* GetTexture();

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	// Member Data:
protected:
	Texture* m_pTexture;
	int m_x;
	int m_y;

	float m_angle;
	int m_centerX;
	int m_centerY;

	int m_width;
	int m_height;

};

#endif // __SPRITE_H__
