#include "Apple.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

Apple::Apple(double x, double y, double z, double width, double height, double depth) : Entity(x, y, z, width, height, depth) {
}

void Apple::render()
{

glPushMatrix();
//glTranslatef(this->getX(), this->getY(), this->getZ());
glBegin(GL_QUADS);
glColor3f(1.0, 0.0, 0.0);
glVertex3f(this->getX() - this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
glVertex3f(this->getX() + this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
glVertex3f(this->getX() + this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
glVertex3f(this->getX() - this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
glEnd();
glPopMatrix();
}
