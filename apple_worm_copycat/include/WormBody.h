#pragma once
#include "Entity.h"
#include "Constants.h"

class WormBody : public Entity{
public:
	WormBody(double x, double y, double z, double width, double height, double depth, Direction direction);
	~WormBody();
	EntityType getType() const override { return EntityType::WORM; }

	// Setters
	void setDirection(Direction direction);
	
	// Getters
	Direction getDirection() const;

	void render() override;

private:
	Direction direction;
};