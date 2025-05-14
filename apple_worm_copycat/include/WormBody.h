#pragma once
#include "Entity.h"
#include "Constants.h"

class WormBody : public Entity{
public:
	WormBody(double x, double y, double z, double width, double height, double depth, Direction direction);
	~WormBody();

	// Setters
	void setDirection(Direction direction);
	
	// Getters
	Direction getDirection() const;


	void render(bool texture) override;

private:
	Direction direction;

};