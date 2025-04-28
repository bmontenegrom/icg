#pragma once
#include "Constants.h"

#include "Entity.h"

class WormHead : public Entity {
public:
	WormHead(float x, float y, float z, float width, float height, float depth, Direction direction);
	~WormHead();
	void render() override;

	// Setters
	void setDirection(Direction direction);

	// Getters
	Direction getDirection() const;
	

private:
	Direction direction;
};