#pragma once
#include "Entity.h"
#include "Worm.h"
#include "Display.h"
#include "Apple.h"
#include <vector>

class Game {
private:
	Worm* worm;
	Apple* apple;
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