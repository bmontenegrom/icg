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
    // Interpolación lineal suave hacia la dirección deseada
    currentOffsetX = currentOffsetX + (targetOffsetX - currentOffsetX) * 0.15f;
    currentOffsetY = currentOffsetY + (targetOffsetY - currentOffsetY) * 0.15f;
    // Tiempo desde último parpadeo
    blinkTimer += 0.016f;  // asumimos ~60fps

    // Iniciar parpadeo cada 10 segundos
    if (!isBlinking && blinkTimer > 20.0f) {
        isBlinking = true;
        blinkTimer = 0.0f;
        blinkProgress = 0.0f;
    }

    // Si estamos en medio del parpadeo, avanzar
    if (isBlinking) {
        blinkProgress += 0.016f;  // avanza 1.0 en un segundo
        if (blinkProgress >= 1.0f) {
            isBlinking = false;
            blinkProgress = 0.0f;
        }
    }

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

    float eyeOpenRatio = 1.0f;
    if (isBlinking) {
        // fase de cierre (0-0.5) y apertura (0.5-1.0)
        if (blinkProgress < 0.5f) {
            eyeOpenRatio = 1.0f - (blinkProgress / 0.5f);  // 1.0 → 0.0
        }
        else {
            eyeOpenRatio = (blinkProgress - 0.5f) / 0.5f;  // 0.0 → 1.0
        }
    }

    // 👉 Ojo izquierdo (esfera blanca)
    glColor3ub(255, 255, 255);
    glPushMatrix();
    // Coordenadas relativas a la cabeza en su sistema escalado
    glTranslated(0.03, 0.25, 0.45);
    glScalef(1.0f, eyeOpenRatio, 1.0f);
    gluSphere(quad, 0.15, 16, 16);
    glPopMatrix();

    // 👉 Pupila (esfera negra)
    glColor3ub(0, 0, 0);
    glPushMatrix();
    glTranslated(0.04 + currentOffsetX, 0.3 + currentOffsetY, 0.52);  // Levemente más al frente
    gluSphere(quad, 0.08, 16, 16);
    glPopMatrix();

    

    

    // 👉 Ojo derecho (esfera blanca)
    glColor3ub(255, 255, 255);
    glPushMatrix();
    glTranslated(+0.25, 0.25, 0.45);  // simétrico en X respecto al ojo izquierdo
    glScalef(1.0f, eyeOpenRatio, 1.0f);
    gluSphere(quad, 0.15, 16, 16);
    glPopMatrix();

    // 👉 Pupila derecha (esfera negra)
    glColor3ub(0, 0, 0);
    glPushMatrix();
    glTranslated(+0.26 + currentOffsetX, 0.3 + currentOffsetY, 0.52);  // más al frente, igual que la izquierda
    gluSphere(quad, 0.08, 16, 16);
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

    float offset = 0.02f; // cuanto se mueve la pupila dentro del ojo
    targetOffsetX = 0.0f;
    targetOffsetY = 0.0f;


    switch (direction) {
    case Direction::UP:
        targetOffsetY = offset;
        break;
    case Direction::DOWN:
        targetOffsetY = -offset;
        break;
    case Direction::LEFT:
        targetOffsetX = -offset;
        break;
    case Direction::RIGHT:
        targetOffsetX = offset;
        break;
    }
}



Direction WormHead::getDirection() const
{
	return this->direction;
    
}
