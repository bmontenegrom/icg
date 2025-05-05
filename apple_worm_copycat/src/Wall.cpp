#include "Wall.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

Wall::Wall(double x, double y, double z, double width, double height, double depth): Entity(x, y, z, width, height, depth)
{
}

void Wall::render()
{
    glPushMatrix();
    glTranslated(getX(), getY(), getZ()); // Trasladar a la posición
    glScaled(getWidth(), getHeight(), getDepth()); // Escalar según las dimensiones

    glBegin(GL_QUADS);

    // Cara frontal
    glColor3f(0.3f, 0.0f, 0.0f); // Rojo oscuro
    glNormal3f(0.0f, 0.0f, 1.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    //glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glNormal3f(0.0f, 0.0f, -1.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    //glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glNormal3f(-1.0f, 0.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    //glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    glNormal3f(1.0f, 0.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
   // glColor3f(0.0f, 1.0f, 1.0f); // Cian
    glNormal3f(0.0f, 1.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
   // glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glNormal3f(0.0f, -1.0f, 0.0f); // Normal apuntando hacia afuera
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);

    glEnd();

    glPopMatrix();

}
