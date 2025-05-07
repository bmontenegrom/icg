#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <iostream>
#include <FreeImage.h>
#include "Apple.h"
#include "Constants.h"

Apple::Apple(double x, double y, double z, double width, double height, double depth) 
    : Entity(x, y, z, width, height, depth), isEaten(false) {
    // Cargar el modelo usando ObjectLoader
    ObjectLoader& loader = ObjectLoader::getInstance();
    vertices = loader.loadOBJ("assets/apple/apple.obj");
    if (vertices.empty()) {
        std::cerr << "Error al cargar el modelo de la manzana" << std::endl;
    }
    texturaManzana = cargarTextura("assets/apple/apple.png");
}

Apple::~Apple() {
}

GLuint Apple::cargarTextura(const char* path) {
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path, 0);
    if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(path);
    if (fif == FIF_UNKNOWN) return 0;

    FIBITMAP* dib = FreeImage_Load(fif, path);
    if (!dib) return 0;

    FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);
    FreeImage_Unload(dib);
    if (!dib32) return 0;

    int width = FreeImage_GetWidth(dib32);
    int height = FreeImage_GetHeight(dib32);
    FreeImage_FlipVertical(dib32);

    BYTE* bits = FreeImage_GetBits(dib32);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(dib32);
    return texID;
}

void Apple::render() {
    if (vertices.empty()) {
        // Si no se pudo cargar el modelo, renderizar un cubo simple
        glPushMatrix();
        glTranslated(getX(), getY(), getZ());
        glScaled(getWidth(), getHeight(), getDepth());

        glBegin(GL_QUADS);
        // Cara frontal
        glColor3f(1.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0.5);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, 0.5);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, 0.5);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, 0.5);
        glEnd();

        glPopMatrix();
        return;
    }

    // Renderizar el modelo 3D
    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(getWidth(), getHeight(), getDepth());

    if (texturaManzana != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaManzana);
    }

    glBegin(GL_TRIANGLES);
    for (const auto& v : vertices) {
        glTexCoord2f(v.texCoord.x, v.texCoord.y);
        glNormal3f(v.normal.x, v.normal.y, v.normal.z);
        glVertex3f(v.position.x, v.position.y, v.position.z);
    }
    glEnd();

    if (texturaManzana != 0) {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

bool Apple::eaten() {
    return this->isEaten;
}

void Apple::setEaten(bool eaten) {
    this->isEaten = eaten;
}



