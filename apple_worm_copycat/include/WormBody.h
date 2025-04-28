#pragma once
#include "Entity.h"
#include "Constants.h"

class WormBody : public Entity{
public:
	WormBody(float x, float y, float z, float width, float height, float depth, Direction direction);
	~WormBody();

	// Setters
	void setDirection(Direction direction);
	
	// Getters
	Direction getDirection() const;


	void render() override;

private:
	Direction direction;

};