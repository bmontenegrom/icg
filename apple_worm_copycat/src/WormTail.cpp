#include "WormTail.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

WormTail::WormTail(float x, float y, float z, float size, Direction direction) : Entity(x, y, z), size(size), direction(direction) {
}

WormTail::~WormTail()
{
}

void WormTail::render()
{
	//float size = 0.3f;
	glPushMatrix();
	//glTranslatef(this->getX(), this->getY(), this->getZ());
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(this->getX() - size / 2, this->getY() - size / 2, 0.);
	glVertex3f(this->getX() + size / 2, this->getY() - size / 2, 0.);
	glVertex3f(this->getX() + size / 2, this->getY() + size / 2, 0.);
	glVertex3f(this->getX() - size / 2, this->getY() + size / 2, 0.);
	glEnd();
	glPopMatrix();
}

void WormTail::setSize(float size)
{
	this->size = size;
}

void WormTail::setDirection(Direction direction)
{
	this->direction = direction;
}

float WormTail::getSize() const
{
	return this->size;
}

Direction WormTail::getDirection() const
{
	return this->direction;
}

