#pragma once

class Entity {
public:
	Entity(float x, float y, float z);
	virtual ~Entity();
	virtual void render() = 0;


	// Setters
	void setPosition(float x, float y, float z);
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	// Getters

	float getX() const;
	float getY() const;
	float getZ() const;

private:
	float x;
	float y;
	float z;


};