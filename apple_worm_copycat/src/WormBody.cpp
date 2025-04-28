#include "WormBody.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>


WormBody::WormBody(float x, float y, float z, float width, float height, float depth, Direction direction) : Entity(x, y, z, width, height, depth), direction(direction)
{
}
WormBody::~WormBody()
{
}



void WormBody::setDirection(Direction direction)
{
	this->direction = direction;
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
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glEnd();
	glPopMatrix();
}
