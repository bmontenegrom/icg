#pragma once
#include <SDL_ttf.h>
#include "Timer.h"
#include <string>

class Hud {
public:
	Hud(SDL_Renderer* renderer, TTF_Font* font);
	~Hud();
	void render(int score, int time);
	void startTime();
	int getTime();

private:
	SDL_Renderer* renderer;
	TTF_Font* font;
	Timer* timer;
	void renderText(const std::string& text, int x, int y, SDL_Color color);

};