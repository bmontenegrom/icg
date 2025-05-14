#include "WormHead.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

WormHead::WormHead(double x, double y, double z, double width, double height, double depth, Direction direction) : Entity(x, y, z, width, height, depth), direction(direction){
}

WormHead::~WormHead()
{
}

void WormHead::render(bool texture)
{
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

    GLUquadric* quad = gluNewQuadric();

    glPushMatrix();  // Cabeza y ojo: misma matriz
    double x = getInterpolatedX();
    double y = getInterpolatedY();
    double z = getInterpolatedZ();
    glTranslated(x, y, z);
    glScaled(getWidth(), getHeight(), getDepth()); // Escala local del objeto

    // 👉 Cabeza (esfera grande verde)
    glColor3ub(0, 128, 0);
    glPushMatrix();
    glTranslated(-0.06, 0.0, 0.0);
    gluSphere(quad, 0.5, 16, 16);
    glPopMatrix();

    // 👉 Ojo izquierdo (esfera blanca)
    glColor3ub(255, 255, 255);
    glPushMatrix();
    // Coordenadas relativas a la cabeza en su sistema escalado
    glTranslated(-0.03, 0.25, 0.45);
    gluSphere(quad, 0.09, 16, 16);
    glPopMatrix();

    // 👉 Pupila (esfera negra)
    glColor3ub(0, 0, 0);
    glPushMatrix();
    glTranslated(-0.03, 0.25, 0.52);  // Levemente más al frente
    gluSphere(quad, 0.04, 16, 16);
    glPopMatrix();

    // 👉 Ojo derecho (esfera blanca)
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslated(+0.2, 0.25, 0.45);  // simétrico en X respecto al ojo izquierdo
    gluSphere(quad, 0.09, 16, 16);
    glPopMatrix();

    // 👉 Pupila derecha (esfera negra)
    glColor3ub(0, 0, 0);
    glPushMatrix();
    glTranslated(+0.2, 0.25, 0.52);  // más al frente, igual que la izquierda
    gluSphere(quad, 0.04, 16, 16);
    glPopMatrix();

    glPopMatrix();  // Fin cabeza + ojo



    gluDeleteQuadric(quad);

    /*// Ojo derecho (blanco)
    glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslated(0.15, 0.1, 0.45);
    gluSphere(quadEye, 0.009, 16, 16);
    glPopMatrix();

    // Pupila derecha (negra)
    glPushMatrix();
    glColor3ub(0, 0, 0);
    glTranslated(0.15, 0.1, 0.5);
    gluSphere(quadEye, 0.005,16, 16);
    glPopMatrix();

    gluDeleteQuadric(quadEye);

    //cuando este andando bien el objloader
    /*
        glBegin(GL_TRIANGLES);
        for (const auto& v : vertices) {
            glTexCoord2f(v.texCoord.x, v.texCoord.y);
            glNormal3f(v.normal.x, v.normal.y, v.normal.z);
            glVertex3f(v.position.x, v.position.y, v.position.z);
        }
        glEnd();
    
    */

}



void WormHead::setDirection(Direction direction)
{
	this->direction = direction;
}



Direction WormHead::getDirection() const
{
	return this->direction;
}
