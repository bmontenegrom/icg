#pragma once
#include "Entity.h"
#include "Worm.h"
#include "Display.h"
#include "Apple.h"
#include "Timer.h"
#include "Camera.h"
#include "Hud.h"
#include "Menu.h"
#include <vector>

class Game {
private:
	Worm* worm;
	Apple* apple;
	std::vector<Entity*> entities;
	Display* display;
	Timer* timer;
	Camera* camera;
	Hud* hud;
	Menu* menu;
	bool isPaused;
	bool isRunning;

public:
	Game();
	~Game();
	void run();
	void init();
	void renderMap();
	void addEntity(Entity* entity);
};