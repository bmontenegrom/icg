#pragma once

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum GameState { MENU, PLAYING, GAME_OVER, PAUSED };

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

// Constante que define la aceleración de la gravedad en unidades por segundo al cuadrado
// Un valor más alto hará que el gusano caiga más rápido
const float GRAVITY = 9.8f;

#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif