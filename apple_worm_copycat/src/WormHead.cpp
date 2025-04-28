#include "WormHead.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

WormHead::WormHead(float x, float y, float z, float width, float height, float depth, Direction direction) : Entity(x, y, z, width, height, depth), direction(direction){
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
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glEnd();
	glPopMatrix();
}



void WormHead::setDirection(Direction direction)
{
	this->direction = direction;
}



Direction WormHead::getDirection() const
{
	return this->direction;
}
