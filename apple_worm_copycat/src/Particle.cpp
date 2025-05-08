#include "Particle.h" 
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <iostream>

Particle::Particle(double x, double y, double z) : Entity(x, y, z)  
{  
   this->setWidth(0.002f);
   this->setHeight(0.002f); 
   this->setDepth(0.002f);
   if (rand() % 2 == 0) {
       this->speedX = (rand() % 10) /1000.0;
   }
   else {
       this->speedX = -(rand() % 10)/1000.0;
   }
   if (rand() % 2 == 0) {
       this->speedY = (rand() % 10)/1000.0;
   }
   else {
       this->speedY =  -(rand() % 10)/1000.0;
   }
   if (rand() % 2 == 0) {
       this->speedZ = (rand() % 10) / 1000.0;
   }
   else {
       this->speedZ =  -(rand() % 10) / 1000.0;
   }

   this->lifeTime = 100;
   this->isAlive = true;
   this->red = 0;
   this->green = 255;
   this->blue = 0;
}  

Particle::~Particle()  
{  
}  

void Particle::render()  
{  
    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(getWidth(), getHeight(), getDepth());

    glBegin(GL_QUADS);

    // Cara frontal
    glColor3f(1.0f, 1.0f, 1.0f); // Rojo
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);

    glEnd();

    glPopMatrix();
}

void Particle::update()
{
	if (this->isAlive) {
		this->setX(this->getX() + this->speedX);
		this->setY(this->getY() + this->speedY);
		this->setZ(this->getZ() + this->speedZ);
		this->lifeTime--;
		if (this->lifeTime <= 0) {
			this->isAlive = false;
		}
	}
}

bool Particle::isDead() const
{
	return !this->isAlive;
}


