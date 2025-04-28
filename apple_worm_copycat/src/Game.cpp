#include "Game.h"
#include <iostream>

Game::Game()
{
	this->display = new Display();
	this->worm = new Worm(0.5f, 0.5f, 0.0f);
	this->apple = new Apple(0.8f, 0.2f, 0.0f, 0.05f, 0.05f, 0.05f);
	this->entities = std::vector<Entity*>();
}

Game::~Game(){
	for (Entity* entity : entities) {
		delete entity;
	}
	delete worm;
	delete apple;
	delete display;
}

void Game::run()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0); // Define un sistema de coordenadas de 0 a 1 en X e Y
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	bool running = true;

	SDL_Event event;

	while (running) {
		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					this->worm->move(UP);
					break;
				case SDLK_DOWN:
					this->worm->move(DOWN);
					break;
				case SDLK_LEFT:
					this->worm->move(LEFT);
					break;
				case SDLK_RIGHT:
					this->worm->move(RIGHT);
					break;
				}
			}
		}
		//FIN MANEJO DE EVENTOS
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		this->worm->render();
		//this->apple->render();
		SDL_GL_SwapWindow(display->getWindow());
	}

}

void Game::init(){
	//agregar carga de mapa etc
}

void Game::renderMap(){
	for (Entity* entity : entities) {
		entity->render();
	}
}

void Game::addEntity(Entity* entity)
{
	this->entities.push_back(entity);
}
