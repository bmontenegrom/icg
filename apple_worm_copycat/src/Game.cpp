#include "Game.h"
#include "Wall.h"
#include <iostream>

Game::Game()
{
	this->display = new Display();
	this->worm = new Worm(0.5f, 0.5f, 0.0f);
	this->apple = new Apple(0.8f, 0.2f, 0.0f, 0.05f, 0.05f, 0.05f);
	this->entities = std::vector<Entity*>();
	this->timer = new Timer();
	this->camera = new Camera(0.0f, 0.0f, 1.5f, 0.5f,  0.5f, 0.0f);
	camera->updateMouseMovement(0, 0); // Inicializa la cámara en la posición deseada
	

	for (int i = 0; i < 21; ++i) {
		Wall* wall = new Wall(0.0 + i * 0.05, 0.025, 0.0, 0.05, 0.05, 0.05);
		this->entities.push_back(wall);
	}
	for (int i = 0; i < 10; ++i) {
		Wall* wall = new Wall(0.7, 0.0 + i* 0.05, 0.0, 0.05, 0.05, 0.05);
		this->entities.push_back(wall);
	}


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
	//gluOrtho2D(0.0, 1.0, 0.0, 1.0); // Define un sistema de coordenadas de 0 a 1 en X e Y
	gluPerspective(35.0, 800.0 / 600.0, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	bool running = true;
	bool mouseButtonPressed = false;

	SDL_Event event;
	
	while (running) {
		
		float timeStep = timer->getTicks() / 1000.0f; // Obtener el tiempo transcurrido en segundos
		if (apple != nullptr && apple->eaten()) {
			delete apple;
			apple = nullptr;
		}
		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					this->worm->move(UP, this->entities, this->apple, timeStep);
					break;
				case SDLK_DOWN:
					this->worm->move(DOWN, this->entities, this->apple, timeStep);
					break;
				case SDLK_LEFT:
					this->worm->move(LEFT, this->entities, this->apple, timeStep);
					break;
				case SDLK_RIGHT:
					this->worm->move(RIGHT, this->entities, this->apple, timeStep);
					break;
				case SDLK_v:
					this->camera->switchCameraMode();
					break;
				case SDLK_ESCAPE:
					running = false;
					break;
				}
			}

			//Manejo de cámara en free mode
			else if (this->camera->getCameraMode() == CameraMode::FREE_CAMERA &&
				event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				mouseButtonPressed = true; // Botón presionado
			}
			else if (this->camera->getCameraMode() == CameraMode::FREE_CAMERA &&
				event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
				mouseButtonPressed = false; // Botón liberado
			}
			else if (event.type == SDL_MOUSEMOTION && mouseButtonPressed &&
				this->camera->getCameraMode() == CameraMode::FREE_CAMERA) {
				SDL_SetRelativeMouseMode(SDL_TRUE);
				// Obtener movimiento relativo del mouse
				int mouseX, mouseY;
				SDL_GetRelativeMouseState(&mouseX, &mouseY);

				// Actualizar la cámara con el movimiento del mouse
				this->camera->updateMouseMovement(mouseX, mouseY);
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
			
			
			
		}
		if (camera->getCameraMode() == CameraMode::THIRD_PERSON) {
			this->camera->followTarget(this->worm->getX(), this->worm->getY(), this->worm->getZ(), 0.4f);

		}
		//FIN MANEJO DE EVENTOS

		this->timer->start();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		//gluLookAt(0.0, 0.0, 1.5, 0.5f, 0.5f, 0.0f, 0.0, 1.0, 0.0);
		camera->applyView();
		this->worm->render();
		this->renderMap();
		if (this->apple != nullptr) {
			this->apple->render();
		}
		
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
