#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <FreeImage.h>
#include "Constants.h"

class Background {
private:
    GLuint backgroundTexture;
    bool loadBackgroundTexture(const char* path);

public:
    Background();
    ~Background();
    void render();
};
