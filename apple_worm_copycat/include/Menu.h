#pragma once

#include "Constants.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <FreeImage.h>


struct TextTextureMenu {
    GLuint textureId;
    int width;
    int height;
};

class Menu {
protected:
    TTF_Font* font;        
    bool isActive;         
    int selectedOption;    
    std::vector<std::string> menuOptions;


    TextTextureMenu createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color);
    void renderTexture2D(const TextTextureMenu& textTex, int x, int y, int screenWidth, int screenHeight);
    
public:
    Menu(TTF_Font* font);
    virtual ~Menu();
    virtual void render() = 0;  
    virtual void handleInput(SDL_Event& event); 
    bool isMenuActive() const;  
    void setActive(bool active);  
    int getSelectedOption() const;  
    virtual void executeSelectedOption() = 0; 
}; 