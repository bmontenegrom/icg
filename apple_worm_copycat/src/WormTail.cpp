#include "WormTail.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

WormTail::WormTail(double x, double y, double z, double width, double height, double depth, Direction direction) : Entity(x, y, z, width, height, depth), direction(direction) {
}

WormTail::~WormTail()
{
}

void WormTail::render(bool texture)
{

    glPushMatrix();
    double x = getInterpolatedX();
    double y = getInterpolatedY();
    double z = getInterpolatedZ();
    glTranslated(x, y, z); // Trasladar a la posición
    glScaled(getWidth(), getHeight(), getDepth()); // Escalar según las dimensiones

	double dx = 0.0, dy = 0.0;
	switch (direction) {
	case UP:    dy = 0.06; break;
	case DOWN:  dy = -0.06; break;
	case LEFT:  dx = -0.06; break;
	case RIGHT: dx = 0.06; break;
	}

    



	GLUquadric* quad = gluNewQuadric();
	glColor3ub(0, 128, 0);
	glTranslated(dx, dy, 0.0);
	gluSphere(quad, 0.55, 16, 16);
	gluDeleteQuadric(quad);

    glPopMatrix();
}

EntityType WormTail::getType() const
{
	return EntityType::WORM;
}



void WormTail::setDirection(Direction direction)
{
	this->direction = direction;
}



Direction WormTail::getDirection() const
{
	return this->direction;
}

