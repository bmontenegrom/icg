#pragma once

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class EntityType {
	WALL,
	OBJECTIVE,
	WORM,
	APPLE
}; 

enum GameState { MENU, PLAYING, GAME_OVER, PAUSED, WINNER };

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;


const float GRAVITY = 9.8f;

#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif