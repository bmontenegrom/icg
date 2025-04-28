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
	double speed;
public:
	Worm(double x, double y, double z);
	~Worm();
	void render() override;

	void move(Direction newDirection);

	// Setters
	void setSpeed(double speed);
	

	// Getters
	int getLength() const;
	double getSpeed() const;
	Direction getHeadDirection() const;


};




