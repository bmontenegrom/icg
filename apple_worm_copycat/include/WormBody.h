#pragma once
#include "Entity.h"
#include "Constants.h"

class WormBody : public Entity{
public:
	WormBody(float x, float y, float z, Direction direction);
	~WormBody();
	void render() override;

private:
	Direction direction;

};