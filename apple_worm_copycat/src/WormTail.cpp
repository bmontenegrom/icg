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
	//double size = 0.3f;
	/*glPushMatrix();
	//glTranslatef(this->getX(), this->getY(), this->getZ());
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
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

    glBegin(GL_QUADS);

    // Cara frontal
    glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glNormal3f(0.0f, 0.0f, 1.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    glNormal3f(1.0f, 0.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
    glNormal3f(0.0f, 1.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
    glNormal3f(0.0f, -1.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);

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

