#include "WormHead.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

WormHead::WormHead(float x, float y, float z, float size, Direction direction) : Entity(x, y, z), size(size), direction(direction){
}

WormHead::~WormHead()
{
}

void WormHead::render()
{
	glPushMatrix();
	//glTranslatef(this->getX(), this->getY(), this->getZ());
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(this->getX() - this->size / 2, this->getY() - this->size / 2, 0.);
	glVertex3f(this->getX() + this->size / 2, this->getY() - this->size / 2, 0.);
	glVertex3f(this->getX() + this->size / 2, this->getY() + this->size / 2, 0.);
	glVertex3f(this->getX() - this->size / 2, this->getY() + this->size / 2, 0.);
	glEnd();
	glPopMatrix();
}

void WormHead::setSize(float size)
{
	this->size = size;
}

void WormHead::setDirection(Direction direction)
{
	this->direction = direction;
}

float WormHead::getSize() const
{
	return this->size;
}

Direction WormHead::getDirection() const
{
	return this->direction;
}
