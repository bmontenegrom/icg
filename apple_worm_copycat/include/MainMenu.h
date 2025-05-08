#pragma once

#include "Menu.h"

class MainMenu : public Menu {
private:
    std::vector<std::string> controls;     // Lista de controles a mostrar

public:
    MainMenu(TTF_Font* font);
    ~MainMenu() override;
    void render() override;
    void handleInput(SDL_Event& event) override;
    void executeSelectedOption() override;
}; 