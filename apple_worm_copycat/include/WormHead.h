#pragma once
#include "Constants.h"

#include "Entity.h"

class WormHead : public Entity {
public:
	WormHead(float x, float y, float z, Direction direction);
	~WormHead();
	void render() override;

private:
	Direction direction;
};