#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <iostream>
#include "Wall.h"
#include "ObjLoader.h"
#include "Constants.h"

Wall::Wall(double x, double y, double z, double width, double height, double depth)
    : Entity(x, y, z, width, height, depth) {
    ObjectLoader& loader = ObjectLoader::getInstance();
    // Escalar el modelo a 1/10000 de su tamaño original
    vertices = loader.loadOBJ("assets/cube/cube.obj", 0.0001f);
    if (vertices.empty()) {
        std::cerr << "Error al cargar el modelo del bloque" << std::endl;
    }
}

void Wall::render() {
    bool usarFallback = vertices.empty();
    if (!usarFallback) {
        // Verifica si todas las UVs son (0,0)
        usarFallback = true;
        for (const auto& v : vertices) {
            if (v.texCoord.x != 0.0f || v.texCoord.y != 0.0f) {
                usarFallback = false;
                break;
            }
        }
    }

    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(getWidth(), getHeight(), getDepth());

    glColor3f(1.0f, 1.0f, 1.0f); // Color blanco

    if (usarFallback) {
        // Si no se pudo cargar el modelo, renderizar un cubo simple
        glBegin(GL_QUADS);
        // Cara frontal
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);

        // Cara trasera
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        // Cara izquierda
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        // Cara derecha
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);

        // Cara superior
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);

        // Cara inferior
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glEnd();
    } else {
        // Renderizar el modelo cargado
        glBegin(GL_TRIANGLES);
        for (const auto& v : vertices) {
            glNormal3f(v.normal.x, v.normal.y, v.normal.z);
            glVertex3f(v.position.x, v.position.y, v.position.z);
        }
        glEnd();
    }

    glPopMatrix();
}
