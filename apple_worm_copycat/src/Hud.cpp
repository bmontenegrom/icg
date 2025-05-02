#include "Hud.h"
#include "Constants.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>

Hud::Hud(TTF_Font* font) :  font(font)
{
    

	this->timer = new Timer();
}

Hud::~Hud()
{
    TTF_CloseFont(font);

}

void Hud::render(int score, int time)
{
    std::string scoreText = "Score: " + std::to_string(score);
    std::string timeText = "Time: " + std::to_string(time) + "s";

    SDL_Color white = { 255, 255, 255, 255 };
    int w, h;

    GLuint scoreTex = createTextTexture(scoreText, font, white, w, h);
    renderTexture2D(scoreTex, 10, 10, w, h, SCREEN_WIDTH, SCREEN_HEIGHT);
    glDeleteTextures(1, &scoreTex);

    GLuint timeTex = createTextTexture(timeText, font, white, w, h);
    renderTexture2D(timeTex, 10, 40, w, h, SCREEN_WIDTH, SCREEN_HEIGHT);
    glDeleteTextures(1, &timeTex);
}

void Hud::startTime()
{
	timer->start();
}

int Hud::getTime()
{
    return timer->getTicks()/1000;
}

GLuint Hud::createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color, int& outWidth, int& outHeight)
{
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Error creando surface: " << TTF_GetError() << std::endl;
        return 0;
    }

    // Convertimos a formato compatible con OpenGL (RGBA8888)
    SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);
    if (!formatted) {
        std::cerr << "Error al convertir superficie a RGBA32: " << SDL_GetError() << std::endl;
        return 0;
    }

    outWidth = formatted->w;
    outHeight = formatted->h;

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        formatted->w, formatted->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, formatted->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(formatted);
    return textureId;
}

void Hud::renderTexture2D(GLuint texture, int x, int y, int width, int height, int screenWidth, int screenHeight)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, screenHeight, 0);  // Coordenadas en píxeles

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1, 1, 1, 1);  // No modificar color

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2i(x, y);
    glTexCoord2f(1, 0); glVertex2i(x + width, y);
    glTexCoord2f(1, 1); glVertex2i(x + width, y + height);
    glTexCoord2f(0, 1); glVertex2i(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


