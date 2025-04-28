#pragma once
#include "Constants.h"
#include "Entity.h"

class WormTail : public Entity
{

public:
    WormTail(float x, float y, float z, float width, float height, float depth, Direction direction);
    ~WormTail();
	void render() override;
	// Setters
	void setDirection(Direction direction);
	// Getters

	Direction getDirection() const;

private:
    Direction direction;

};