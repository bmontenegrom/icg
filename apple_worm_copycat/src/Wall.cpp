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
    glColor3f(0.3f, 0.0f, 0.0f); // Rojo
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    //glColor3f(0.0f, 1.0f, 0.0f); // Verde
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    //glColor3f(0.0f, 0.0f, 1.0f); // Azul
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    //glColor3f(1.0f, 1.0f, 0.0f); // Amarillo
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
   // glColor3f(0.0f, 1.0f, 1.0f); // Cian
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
   // glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);

    glEnd();

    glPopMatrix();

}
