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
	/*glPushMatrix();
	//glTranslatef(this->getX(), this->getY(), this->getZ());
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() - this->getHeight() / 2, 0.);
	glVertex3f(this->getX() + this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glVertex3f(this->getX() - this->getWidth() / 2, this->getY() + this->getHeight() / 2, 0.);
	glEnd();
	glPopMatrix();
	*/

    glPushMatrix();
    double x = getInterpolatedX();
    double y = getInterpolatedY();
    double z = getInterpolatedZ();
    glTranslated(x, y, z);
    glScaled(getWidth(), getHeight(), getDepth()); // Escalar según las dimensiones

	// Dibujar el cuerpo: círculo mediano, verde claro
	GLUquadric* quad = gluNewQuadric();

	glColor3ub(173, 255, 47); // verde claro
	glTranslated(0.0, 0.0, 0.0);
	gluSphere(quad, 0.5, 16, 16);

	gluDeleteQuadric(quad);

    glPopMatrix();
}
