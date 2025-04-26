#pragma once
#include "Entity.h"
#include "Constants.h"

class WormBody : public Entity{
public:
	WormBody(float x, float y, float z, float size, Direction direction);
	~WormBody();

	// Setters
	void setSize(float size);
	void setDirection(Direction direction);
	
	// Getters
	float getSize() const;
	Direction getDirection() const;


	void render() override;

private:
	Direction direction;
	float size;

};