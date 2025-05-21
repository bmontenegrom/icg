#include "Objective.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>

#include <Game.h>

Objective::Objective(double x, double y, double z, double width, double height, double depth, Game* game) 
    : Entity(x, y, z, width, height, depth), game(game)
{
    float particleX = x; 
    float particleY = y; 
    float particleZ = z;
	this->isPaused = false;
	this->manzanaComida = false;
	for (int i = 0; i < 50; ++i) {
		Particle* particle = new Particle(particleX, particleY, particleZ);
		particles.push_back(particle);
	}
}
Objective::~Objective()
{
	for (Particle* particle : particles) {
		delete particle;
	}
	particles.clear();
}

void Objective::render(bool texture)
{
    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(0.05, 0.05, 0.05);  // Tamaño fijo de 0.05 x 0.05 x 0.05

    glBegin(GL_QUADS);
    // Cara frontal
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glPopMatrix();


    renderParticles( texture);
}

void Objective::renderParticles( bool texture)
{
    
    

    
    
    for (int i = 0; i < particles.size(); ++i) {
        if (particles[i]->isDead()){
            delete particles[i];
            float particleX = getX();
            float particleY = getY();
            float particleZ = getZ();
            particles[i] = new Particle(particleX, particleY, particleZ);
        }
    }
	if (manzanaComida) {
        for (Particle* particle : particles) {
            if (!isPaused) {
                particle->update();
            }
            particle->render(texture);
        }
	}
    
}

void Objective::setPaused(bool paused)
{
	this->isPaused = paused;
}

void Objective::SetManzanaComida(bool comida)
{
	this->manzanaComida = comida;
}

EntityType Objective::getType() const
{
    return EntityType::OBJECTIVE;
}
