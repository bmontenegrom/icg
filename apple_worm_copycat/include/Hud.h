#pragma once
#include <SDL.h>
#include "SDL_opengl.h"
#include <GL/glu.h>
#include <SDL_ttf.h>
#include "Timer.h"
#include <string>
#include <unordered_map>


struct TextTexture {
	GLuint textureId;
	int width;
	int height;
};


class Hud {
public:
	Hud(TTF_Font* font);
	~Hud();
	void render(int score, int time, float gameSpeed);
	void startTime();
	int getTime();
	
private:
	TTF_Font* font;
	Timer* timer;

	GLuint createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color, int& outWidth, int& outHeight);
	void renderTexture2D(GLuint texture, int x, int y, int width, int height, int screenWidth, int screenHeight);
	

};