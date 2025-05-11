#pragma once
#include "Display.h"
#include "Timer.h"
#include "Camera.h"
#include "Hud.h"
#include "MainMenu.h"
#include "GameOverMenu.h"
#include "Level.h"

#include <vector>

class Game {
private:
	Display* display;
	Timer* timer;
	Camera* camera;
	Hud* hud;
	MainMenu* mainMenu;
	GameOverMenu* gameOverMenu;
	std::vector<Level*> levels;
	int currentLevel;
	bool isPaused;
	bool isRunning;
	bool hasStartedPlaying;
	GameState currentState;
	float gameSpeedMultiplier;
	
	void updateCamera(Worm* worm);
	void loadLevel(int levelNumber);
	void nextLevel();

public:
	int score;
	Game();
	~Game();
	void run();
	void handleKeyPress(SDL_Keycode key, Worm* worm, Apple* apple, float timeStep, bool& wireframe, bool& texture, bool& smooth);
	void resetGame();
	void setScore(int newScore);
	int getScore() const;
	void changeState(GameState newState);
	void setGameSpeed(float speed);
	float getGameSpeed() const;
};