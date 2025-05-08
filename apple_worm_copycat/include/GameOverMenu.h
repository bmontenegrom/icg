#pragma once

#include "Menu.h"

class GameOverMenu : public Menu {
private:
    int finalScore;  // Puntuación final del juego

public:
    GameOverMenu(TTF_Font* font);
    ~GameOverMenu() override;
    void render() override;
    void handleInput(SDL_Event& event) override;
    void setFinalScore(int score);  // Establece la puntuación final
    void executeSelectedOption() override;
}; 