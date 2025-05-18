#pragma once
#include "Menu.h"

class WinnerMenu : public Menu {
private:
    int finalScore;
    int finalTime;
public:
    WinnerMenu(TTF_Font* font);
    ~WinnerMenu() override;
    void render() override;
    void handleInput(SDL_Event& event) override;
    void setFinalScore(int score);
    void setFinalTime(int time);
    void executeSelectedOption() override;
}; 