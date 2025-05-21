#include "Background.h"
#include <iostream>

Background::Background() : backgroundTexture(0) {
    if (!loadBackgroundTexture("assets/background.png")) {
        std::cerr << "Error al cargar la textura de fondo" << std::endl;
    }
}

Background::~Background() {
    if (backgroundTexture != 0) {
        glDeleteTextures(1, &backgroundTexture);
    }
}

bool Background::loadBackgroundTexture(const char* path) {
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path, 0);
    if (fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(path);
    }
    if (fif == FIF_UNKNOWN) {
        return false;
    }

    FIBITMAP* dib = FreeImage_Load(fif, path);
    if (!dib) {
        return false;
    }

    FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);
    FreeImage_Unload(dib);
    if (!dib32) {
        return false;
    }

    int width = FreeImage_GetWidth(dib32);
    int height = FreeImage_GetHeight(dib32);

    FreeImage_FlipVertical(dib32);

    BYTE* bits = FreeImage_GetBits(dib32);

    glGenTextures(1, &backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, bits);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    FreeImage_Unload(dib32);
    return true;
}

void Background::render() {

    if (backgroundTexture == 0) { 
        return;
    }
    // 1. Desactiva el depth test y dibuja el fondo
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(SCREEN_WIDTH, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, SCREEN_HEIGHT);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // 2. Limpia el depth buffer y reactiva el depth test
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}