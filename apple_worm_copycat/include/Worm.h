#pragma once
#include "Entity.h"
#include <vector>
#include "Constants.h"

class WormHead;
class WormBody;
class WormTail;

class Worm : public Entity {
private:
	WormHead* head;
	WormTail* tail;
	std::vector<Entity*> body;
	int length;
	float speed;
public:
	Worm(float x, float y, float z);
	~Worm();
	void render() override;

	void move(Direction newDirection);

	// Setters
	void setSpeed(float speed);
	

	// Getters
	int getLength() const;
	float getSpeed() const;
	Direction getHeadDirection() const;


};




