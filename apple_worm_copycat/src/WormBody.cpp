#include "WormBody.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>


WormBody::WormBody(double x, double y, double z, double width, double height, double depth, Direction direction) : Entity(x, y, z, width, height, depth), direction(direction)
{
}
WormBody::~WormBody()
{
}

EntityType WormBody::getType() const
{
    return EntityType::WORM;
}



void WormBody::setDirection(Direction direction)
{
	this->direction = direction;
}




Direction WormBody::getDirection() const
{
	return Direction();
}



void WormBody::render(bool texture)
{
	

    glPushMatrix();
    double x = getInterpolatedX();
    double y = getInterpolatedY();
    double z = getInterpolatedZ();
    glTranslated(x, y, z); // Trasladar a la posición
    glScaled(getWidth(), getHeight(), getDepth()); // Escalar según las dimensiones

    

	GLUquadric* quad = gluNewQuadric();



	glColor3ub(173, 255, 47); // verde claro
	glTranslated(0.0, 0.0, 0.0);
	gluSphere(quad, 0.53, 16, 16);

	gluDeleteQuadric(quad);

    glPopMatrix();
}
