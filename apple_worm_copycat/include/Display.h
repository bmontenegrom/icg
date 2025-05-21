#pragma once
#include <SDL.h>
#include "SDL_opengl.h"
#include <GL/glu.h>

class Display {
public:
	Display();
	~Display();
	void changeLightPosition(); // Función para cambiar la posición de la luz
	void changeLightColor(); // Función para cambiar el color de la luz
	void resetPerspective(); 
	void swapWindow();
private:
	SDL_Window* window;
	SDL_GLContext context;
	int width;
	int height;
	void init();
	
	// Variables para controlar la luz
	int currentLightPosition; // Índice de la posición actual de la luz
	int currentLightColor; // Índice del color actual de la luz
};
