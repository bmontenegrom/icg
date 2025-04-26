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
	int size;
	float speed;
	Direction direction;
public:
	Worm(float x, float y, float z, Direction direction);
	~Worm();
	void render() override;

};




