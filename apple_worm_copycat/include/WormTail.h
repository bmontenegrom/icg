#pragma once
#include "Constants.h"
#include "Entity.h"

class WormTail : public Entity
{

public:
    WormTail(double x, double y, double z, double width, double height, double depth, Direction direction);
    ~WormTail();
	void render(bool texture) override;
	EntityType getType() const override;

	void setDirection(Direction direction);
	

	Direction getDirection() const;

private:
    Direction direction;

};