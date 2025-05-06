#include "Game.h"
#include "Wall.h"
#include "Objective.h"
#include <iostream>

Game::Game()
{
	this->display = new Display();
	this->worm = new Worm(0.5f, 0.5f, 0.0f);
	this->apple = new Apple(0.8f, 0.2f, 0.0f, 0.05f, 0.05f, 0.05f);
	this->entities = std::vector<Entity*>();
	this->timer = new Timer();
	// Posición de la cámara: arriba y a la derecha
	// Punto al que mira: centro del área de juego (aproximadamente 0.5, 0.25, 0.0)
	this->camera = new Camera();
	this->isPaused = false;
	this->isRunning = true;  // Inicializar el estado de ejecución del juego
	//camera->updateMouseMovement(0, 0); // Inicializa la cámara en la posición deseada

	if (TTF_Init() != 0) {
		std::cerr << "Error inicializando SDL_ttf: " << TTF_GetError() << std::endl;
		exit(1);
	}

	TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
	if (font == nullptr) {
		std::cerr << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
		exit(1);
	}
	this->hud = new Hud(font);
	this->menu = new Menu(font);  // Crear el menú con la misma fuente

	for (int i = 0; i < 21; ++i) {
		Wall* wall = new Wall(0.0 + i * 0.05, 0.025, 0.0, 0.05, 0.05, 0.05);
		this->entities.push_back(wall);
	}
	for (int i = 0; i < 10; ++i) {
		Wall* wall = new Wall(0.7, 0.0 + i* 0.05, 0.0, 0.05, 0.05, 0.05);
		this->entities.push_back(wall);
	}

	Objective* objective = new Objective(0.8f, 0.8f, 0.0f, 0.05f, 0.05f, 0.05f);
	this->entities.push_back(objective);


}

Game::~Game(){
	for (Entity* entity : entities) {
		delete entity;
	}
	delete worm;
	delete apple;
	delete display;
	delete menu;  // Liberar la memoria del menú
}

void Game::run()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0.0, 1.0, 0.0, 1.0); // Define un sistema de coordenadas de 0 a 1 en X e Y
	gluPerspective(35.0, 800.0 / 600.0, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	bool mouseButtonPressed = false;
	bool wireframe = false;
	bool texture = true;

	SDL_Event event;
	this->hud->startTime();
	int score = 10;

	while (isRunning) {  // Usar isRunning en lugar de running
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(35.0, 800.0 / 600.0, 0.1, 100.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		float timeStep = timer->getTicks() / 1000.0f; // Obtener el tiempo transcurrido en segundos
		if (menu->isMenuActive()) {
			menu->handleInput(event);
			if (!menu->isMenuActive()) {
				if (menu->getSelectedOption() == 1) { // Salir
					isRunning = false;
				}
			}
		}
		else if (apple != nullptr && apple->eaten()) {
			delete apple;
			apple = nullptr;
		}

		// Actualiza la física de la gravedad para el gusano en cada frame
		// Esto hace que el gusano caiga si no está sobre un bloque
		if (!isPaused) {
			this->worm->updateGravity(this->entities, timeStep);
		}

		//MANEJO DE EVENTOS
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			else if (menu->isMenuActive()) {
				menu->handleInput(event);
				if (!menu->isMenuActive()) {
					if (menu->getSelectedOption() == 1) {
						isRunning = false;
					}
				}
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					if (!isPaused) {
						this->worm->move(UP, this->entities, this->apple, timeStep);
					}
					break;
				case SDLK_DOWN:
					if (!isPaused) {
						this->worm->move(DOWN, this->entities, this->apple, timeStep);
					}
					break;
				case SDLK_LEFT:
					if (!isPaused) {
						this->worm->move(LEFT, this->entities, this->apple, timeStep);
					}
					break;
				case SDLK_RIGHT:
					if (!isPaused) {
						this->worm->move(RIGHT, this->entities, this->apple, timeStep);
					}
					break;
				case SDLK_p:
					isPaused = !isPaused;
					for (Entity* entity : entities) {
						if (Objective* objective = dynamic_cast<Objective*>(entity)) {
							objective->setPaused(isPaused);
							break;
						}
					}
					break;
				case SDLK_v:
					this->camera->switchCameraMode();
					if (this->camera->getCameraMode() == CameraMode::FREE_CAMERA || this->camera->getCameraMode() == CameraMode::FIRST_PERSON) {
						SDL_SetRelativeMouseMode(SDL_TRUE); // Activar modo relativo al entrar en free cam o first person
					} else {
						SDL_SetRelativeMouseMode(SDL_FALSE); // Desactivar modo relativo al salir
					}
					break;
				case SDLK_q:
					isRunning = false;
					break;
				case SDLK_w:
					wireframe = !wireframe;
					if (wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}
					else {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
					break;
				case SDLK_t:
					texture = !texture;
					break;
				case SDLK_l:
					// Cambiar la posición de la luz
					this->display->changeLightPosition();
					break;
				case SDLK_k:
					// Cambiar el color de la luz
					this->display->changeLightColor();
					break;
				case SDLK_ESCAPE:
					menu->setActive(true);
					break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION && this->camera->getCameraMode() == CameraMode::FREE_CAMERA) {
				// Obtener movimiento relativo del mouse
				int mouseX, mouseY;
				SDL_GetRelativeMouseState(&mouseX, &mouseY);

				// Actualizar la cámara con el movimiento del mouse
				this->camera->updateMouseMovement(mouseX, mouseY);
			}
		}
		if (camera->getCameraMode() == CameraMode::THIRD_PERSON || camera->getCameraMode() == CameraMode::FIRST_PERSON) {
			// Obtener dirección del gusano
			Direction dir = this->worm->getHeadDirection();
			float dirX = 0.0f, dirY = 0.0f, dirZ = 0.0f;
			switch (dir) {
				case UP: dirY = 1.0f; break;
				case DOWN: dirY = -1.0f; break;
				case LEFT: dirX = -1.0f; break;
				case RIGHT: dirX = 1.0f; break;
			}
			this->camera->followTarget(
				this->worm->getX(),
				this->worm->getY(),
				this->worm->getZ(),
				0.25f, // distancia para third person
				dirX, dirY, dirZ
			);
		}
		//FIN MANEJO DE EVENTOS

		

		this->timer->start();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		//gluLookAt(0.0, 0.0, 1.5, 0.5f, 0.5f, 0.0f, 0.0, 1.0, 0.0);
		camera->applyView();

		if (!menu->isMenuActive()) {  // Solo renderizar el juego si el menú no está activo
			this->worm->render();
			this->renderMap();
			if (this->apple != nullptr) {
				this->apple->render();
			}
			
			hud->render(score, this->hud->getTime());
		}

		menu->render();  // Renderizar el menú
		
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
