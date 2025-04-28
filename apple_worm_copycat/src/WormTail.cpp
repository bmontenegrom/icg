#include "WormTail.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

WormTail::WormTail(float x, float y, float z, float width, float height, float depth, Direction direction) : Entity(x, y, z, width, height, depth), direction(direction) {
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
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glEnd();
	glPopMatrix();
}



void WormTail::setDirection(Direction direction)
{
	this->direction = direction;
}



Direction WormTail::getDirection() const
{
	return this->direction;
}

