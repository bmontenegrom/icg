#pragma once
#include <SDL.h>
#include "SDL_opengl.h"
#include <GL/glu.h>

class Display {
public:
	Display();
	~Display();
	void changeLightPosition();
	void changeLightColor(); 
	void resetPerspective(); 
	void swapWindow();
private:
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;
	void init();
	

	int currentLightPosition; 
	int currentLightColor; 
};
