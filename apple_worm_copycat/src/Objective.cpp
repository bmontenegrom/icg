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
    
    spinAngle += 1.5f;  
    if (spinAngle >= 360.0f) spinAngle -= 360.0f;

    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(0.08, 0.08, 0.08);  

    
    GLUquadric* quad = gluNewQuadric();
    glColor3ub(0, 0, 0);  
    gluDisk(quad, 0.0, 0.4, 32, 1);


    glColor3ub(30, 30, 30);  
    gluDisk(quad, 0.4, 0.5, 32, 1);


    glPushMatrix();
    glRotated(spinAngle, 0.0, 0.0, 1.0);  
    glBegin(GL_LINES);
    glColor3ub(255, 255, 255);  

    int lineCount = 6;
    for (int i = 0; i < lineCount; ++i) {
        float angle = 2 * M_PI * i / lineCount;
        float x = cos(angle) * 0.5;
        float y = sin(angle) * 0.5;
        glVertex2f(0.0f, 0.0f);  
        glVertex2f(x, y);       
    }
    glEnd();
    glPopMatrix();


    gluDeleteQuadric(quad);
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
