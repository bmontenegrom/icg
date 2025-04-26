#include "WormBody.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>


WormBody::WormBody(float x, float y, float z, float size, Direction direction) : Entity(x, y, z), size(size), direction(direction)
{
}
WormBody::~WormBody()
{
}

void WormBody::setSize(float size){
	this->size = size;
}

void WormBody::setDirection(Direction direction)
{
	this->direction = direction;
}


float WormBody::getSize() const
{
	return this->size;
}

Direction WormBody::getDirection() const
{
	return Direction();
}



void WormBody::render()
{
	glPushMatrix();
	//glTranslatef(this->getX(), this->getY(), this->getZ());
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(this->getX() - this->size/2, this->getY() - this->size/2, 0.);
	glVertex3f(this->getX() + this->size/2, this->getY() - this->size/2, 0.);
	glVertex3f(this->getX() + this->size/2, this->getY() + this->size/2, 0.);
	glVertex3f(this->getX() - this->size/2, this->getY() + this->size/2, 0.);
	glEnd();
	glPopMatrix();
}
