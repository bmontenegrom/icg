#include "Particle.h" 
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include <iostream>

Particle::Particle(double x, double y, double z) : Entity(x, y, z)  
{  
   this->setWidth(0.001f);
   this->setHeight(0.001f); 
   this->setDepth(0.001f);
   if (rand() % 2 == 0) {
       this->speedX = (rand() % 5) /1000.0;
   }
   else {
       this->speedX = -(rand() % 5)/1000.0;
   }
   if (rand() % 2 == 0) {
       this->speedY = (rand() % 5)/1000.0;
   }
   else {
       this->speedY =  -(rand() % 5)/1000.0;
   }
   if (rand() % 2 == 0) {
       this->speedZ = (rand() % 5) / 1000.0;
   }
   else {
       this->speedZ =  -(rand() % 5) / 1000.0;
   }

   this->lifeTime = 50;
   this->isAlive = true;
   this->red = rand() / 256;
   this->green = rand() / 256;
   this->blue = rand() / 256;
}  

Particle::~Particle()  
{  
}  

void Particle::render()  
{  
    glPushMatrix();
    glTranslated(getX(), getY(), getZ()); // Trasladar a la posición
    glScaled(getWidth(), getHeight(), getDepth()); // Escalar según las dimensiones

    glBegin(GL_QUADS);

    //todo mejorar colores
    // Cara frontal
    glColor3f(1.0, 1.0f, 1.0f); // Rojo
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    //glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    //glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    //glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
   // glColor3f(0.0f, 1.0f, 1.0f); // Cian
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
   // glColor3f(1.0f, 0.0f, 1.0f); // Magenta
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


