#include "Display.h"
#include "Constants.h"
#include <iostream>

Display::Display(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "No se pudo iniciar SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}
	window = SDL_CreateWindow("AppleWorm",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	
	if (!window) {
		std::cerr << "No se pudo crear la ventana: " << SDL_GetError() << std::endl;
		exit(1);
	}
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer) {
		std::cerr << "No se pudo crear el renderer: " << SDL_GetError() << std::endl;
		exit(1);
	}
	context = SDL_GL_CreateContext(window);
	if (!context) {
		std::cerr << "No se pudo crear el contexto OpenGL: " << SDL_GetError() << std::endl;
		exit(1);
	}; 
	this->init();
}

Display::~Display(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Window* Display::getWindow()
{
	return this->window;
}

SDL_GLContext Display::getContext()
{
	return context;
}

SDL_Renderer* Display::getRenderer()
{
	return this->renderer;
}

void Display::init(){
	glMatrixMode(GL_PROJECTION);
	glClearColor(0, 0, 0, 1);
	gluPerspective(45, SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
}

