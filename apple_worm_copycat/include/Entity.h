#pragma once

class Entity {
public:
	Entity(float x, float y, float z, float width, float height, float depth);
	Entity(float x, float y, float z);
	virtual ~Entity();
	virtual void render() = 0;


	// Setters
	void setPosition(float x, float y, float z);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setWidth(float width);
	void setHeight(float height);
	void setDepth(float depth);

	// Getters

	float getX() const;
	float getY() const;
	float getZ() const;
	float getWidth() const;
	float getHeight() const;
	float getDepth() const;


	// Check if the entity is colliding with another entity
	bool isColliding(const Entity& other) const;

private:
	//center position
	float x;
	float y;
	float z;
	//hitbox
	float width;
	float height;
	float depth;


};