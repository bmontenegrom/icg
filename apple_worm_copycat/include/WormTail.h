#pragma once
#include "Constants.h"
#include "Entity.h"

class WormTail : public Entity
{

public:
    WormTail(float x, float y, float z, Direction direction);
    ~WormTail();
	void render() override;

private:
    Direction direction;

};