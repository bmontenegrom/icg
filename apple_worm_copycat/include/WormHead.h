#pragma once
#include "Constants.h"

#include "Entity.h"

class WormHead : public Entity {
public:
	WormHead(double x, double y, double z, double width, double height, double depth, Direction direction);
	~WormHead();
	void render() override;

	// Setters
	void setDirection(Direction direction);

	// Getters
	Direction getDirection() const;
	

private:
	Direction direction;
};