#include "Hud.h"
#include <iostream>

Hud::Hud(SDL_Renderer* renderer, TTF_Font* font) : renderer(renderer), font(font)
{
    if (!renderer) {
        std::cerr << "Renderer inválido al inicializar HUD." << std::endl;
    }

	this->timer = new Timer();
}

Hud::~Hud()
{
    TTF_CloseFont(font);

}

void Hud::render(int score, int time)
{
    SDL_Color white = { 255, 0, 0 };
    renderText("Test HUD", 50, 50, { 255, 255, 255 });
   // renderText("Score: " + std::to_string(score), 10, 10, white);
    //renderText("Time: " + std::to_string(time), 10, 90, white);
}

void Hud::startTime()
{
	timer->start();
}

int Hud::getTime()
{
    return timer->getTicks();
}

void Hud::renderText(const std::string& text, int x, int y, SDL_Color color)
{

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Error renderizando texto: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Error creando textura: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect rect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


}
