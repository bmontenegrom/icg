#include "Game.h"

Game::Game()
{
	this->display = new Display();
	this->worm = new Worm(0, 0, 0, UP);
	this->entities = std::vector<Entity*>();
}

Game::~Game(){
	for (Entity* entity : entities) {
		delete entity;
	}
	delete display;
}

void Game::run()
{
	bool fin = false;
	bool rotate = false;

	SDL_Event evento;

	float x, y, z;

	x = 0;
	y = 0;
	z = 5;
	float degrees = 0;
	while (!fin) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);

		if (rotate) {
			degrees = degrees + 0.1f;
		}
		glRotatef(degrees, 0.0, 1.0, 0.0);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(1., -1., 0.);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-1., -1., 0.);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0., 1., 0.);
		glEnd();
		//FIN DIBUJAR OBJETOS

		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&evento)) {
			switch (evento.type) {
			case SDL_MOUSEBUTTONDOWN:
				rotate = true;
				break;
			case SDL_MOUSEBUTTONUP:
				rotate = false;
				break;
			case SDL_QUIT:
				fin = true;
				break;
			case SDL_KEYUP:
				switch (evento.key.keysym.sym) {
				case SDLK_ESCAPE:
					fin = true;
					break;
				case SDLK_RIGHT:
					break;
				}
			}
		}
		//FIN MANEJO DE EVENTOS
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
