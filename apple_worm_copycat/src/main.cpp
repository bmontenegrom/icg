#include "SDL.h"
#include "SDL_opengl.h"
#include <iostream>
#include "FreeImage.h"
#include <stdio.h>
#include <conio.h>
#include <GL/glu.h>
#include "Game.h"

using namespace std;

int main(int argc, char* argv[]) {
	Game* game = new Game();
	game->init();
	game->run();

	return 0;
}
