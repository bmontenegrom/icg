#pragma once
#include <SDL.h>
#include "SDL_opengl.h"
#include <GL/glu.h>

class Display {
public:
	Display();
	~Display();
	SDL_Window* getWindow();
private:
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;
	void init();

};
