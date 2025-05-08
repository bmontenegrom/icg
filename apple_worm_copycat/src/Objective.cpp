#include "Objective.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>

Objective::Objective(double x, double y, double z, double width, double height, double depth) : Entity(x, y, z, width, height, depth)
{
    float particleX = x; //- width / 2 + (rand()% 5)/100.f;
    float particleY = y; //- height / 2 + (rand() % 5) / 100.f;
    float particleZ = z; //- depth / 2 + (rand() % 5) / 100.f;
	this->isPaused = false;
	for (int i = 0; i < 100; ++i) {
		Particle* particle = new Particle(particleX, particleY, particleZ);
		particles.push_back(particle);
	}
}
Objective::~Objective()
{
}

void Objective::render()
{
    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(0.05, 0.05, 0.05);  // Tamaño fijo de 0.05 x 0.05 x 0.05

    glBegin(GL_QUADS);
    // Cara frontal
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glEnd();

    glPopMatrix();

    renderParticles();
}

void Objective::renderParticles()
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

	for (Particle* particle : particles) {
		if (!isPaused) {
			particle->update();
		}
		particle->render();
	}
}

void Objective::setPaused(bool paused)
{
	this->isPaused = paused;
}
