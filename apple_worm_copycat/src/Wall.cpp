#include "Wall.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

Wall::Wall(double x, double y, double z, double width, double height, double depth): Entity(x, y, z, width, height, depth)
{
}

void Wall::render()
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(0.3, 0.0, 0.0);
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glEnd();
	glPopMatrix();

}
