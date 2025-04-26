#pragma once
#include "Entity.h"
#include "Worm.h"
#include "Display.h"
#include <vector>

class Game {
private:
	Worm* worm;
	std::vector<Entity*> entities;
	Display* display;



public:
	Game();
	~Game();
	void run();
	void init();
	void renderMap();
	void addEntity(Entity* entity);
};