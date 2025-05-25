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
	

	
	context = SDL_GL_CreateContext(window);
	if (!context) {
		std::cerr << "No se pudo crear el contexto OpenGL: " << SDL_GetError() << std::endl;
		exit(1);
	}; 
	
	currentLightPosition = 0;
	currentLightColor = 0;
	
	this->init();
}

Display::~Display(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



void Display::init(){
	glMatrixMode(GL_PROJECTION);
	glClearColor(0, 0, 0, 1);
	gluPerspective(45, SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	
	
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	
	
	GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	gluLookAt(
		12.0f, 15.0f, 12.0f,  
		0.0f, 0.0f, 0.0f,     
		0.0f, 1.0f, 0.0f      
	);
}

void Display::changeLightPosition() {

    GLfloat positions[3][4] = {
        { 5.0f, 5.0f, 5.0f, 1.0f }, 
        { -5.0f, 5.0f, -5.0f, 1.0f }, 
        { 0.0f, 10.0f, 0.0f, 1.0f }   
    };
    
    
    currentLightPosition = (currentLightPosition + 1) % 3;
    
    
    glLightfv(GL_LIGHT0, GL_POSITION, positions[currentLightPosition]);
}

void Display::changeLightColor() {
    
    GLfloat colors[3][4] = {
        { 0.8f, 0.8f, 0.8f, 1.0f },  // Blanco
        { 0.8f, 0.4f, 0.4f, 1.0f },  // Rojo suave
        { 0.4f, 0.8f, 0.4f, 1.0f }   // Verde suave
    };
    
    
    currentLightColor = (currentLightColor + 1) % 3;
    
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, colors[currentLightColor]);
}

void Display::resetPerspective()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0, SCREEN_WIDTH / (double)SCREEN_HEIGHT, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Display::swapWindow()
{
	SDL_GL_SwapWindow(window);
}

