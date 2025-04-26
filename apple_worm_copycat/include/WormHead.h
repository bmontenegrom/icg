#pragma once
#include "Constants.h"

#include "Entity.h"

class WormHead : public Entity {
public:
	WormHead(float x, float y, float z, float size, Direction direction);
	~WormHead();
	void render() override;

	// Setters
	void setSize(float size);
	void setDirection(Direction direction);

	// Getters
	float getSize() const;
	Direction getDirection() const;
	

private:
	Direction direction;
	float size;
};