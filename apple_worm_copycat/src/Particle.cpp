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
   
   // Aumentamos significativamente la velocidad base de las partículas
   if (rand() % 2 == 0) {
       this->speedX = (rand() % 10) / 100.0; // Aumentamos 10 veces la velocidad base
   }
   else {
       this->speedX = -(rand() % 10) / 100.0;
   }
   if (rand() % 2 == 0) {
       this->speedY = (rand() % 10) / 100.0;
   }
   else {
       this->speedY = -(rand() % 10) / 100.0;
   }
   if (rand() % 2 == 0) {
       this->speedZ = (rand() % 10) / 100.0;
   }
   else {
       this->speedZ = -(rand() % 10) / 100.0;
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

void Particle::render(bool texture)  
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

void Particle::update(float speedMultiplier)
{
    if (this->isAlive) {
        // Debug: imprimir la velocidad actual
        /*static int frameCount = 0;
        if (frameCount++ % 60 == 0) { // Imprimir cada 60 frames
            std::cout << "Particle Speed Multiplier: " << speedMultiplier 
                      << " Base Speed: " << this->speedX 
                      << " Adjusted Speed: " << (this->speedX * speedMultiplier) << std::endl;
        }*/

        // Aplicamos el multiplicador de velocidad al movimiento
        float adjustedSpeedX = this->speedX * speedMultiplier;
        float adjustedSpeedY = this->speedY * speedMultiplier;
        float adjustedSpeedZ = this->speedZ * speedMultiplier;
        
        this->setX(this->getX() + adjustedSpeedX);
        this->setY(this->getY() + adjustedSpeedY);
        this->setZ(this->getZ() + adjustedSpeedZ);
        
        // Ajustamos el tiempo de vida según la velocidad
        this->lifeTime -= speedMultiplier;
        if (this->lifeTime <= 0) {
            this->isAlive = false;
        }
    }
}

bool Particle::isDead() const
{
	return !this->isAlive;
}


