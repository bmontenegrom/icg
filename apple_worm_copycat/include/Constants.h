#pragma once
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum CameraMode
{
	ISOMETRIC,
	THIRD_PERSON,
	FREE_CAMERA
};

// Constante que define la aceleración de la gravedad en unidades por segundo al cuadrado
// Un valor más alto hará que el gusano caiga más rápido
const float GRAVITY = 9.8f;
