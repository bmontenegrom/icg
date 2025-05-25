#include "GameOverMenu.h"
#include <iostream>

GameOverMenu::GameOverMenu(TTF_Font* font) : Menu(font), finalScore(0) {
   
    menuOptions = {"Jugar de nuevo", "Volver al menú"};
}

GameOverMenu::~GameOverMenu() {
}

void GameOverMenu::setFinalScore(int score) {
    finalScore = score;
}

void GameOverMenu::render() {
    if (!isActive) return;

    
    glDisable(GL_LIGHTING);

    
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);

    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    
    float panelWidth = 400;
    float panelHeight = 400;
    float panelX = (SCREEN_WIDTH - panelWidth) / 2;
    float panelY = (SCREEN_HEIGHT - panelHeight) / 2;

    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   
    glBegin(GL_QUADS);
    glColor4f(0.7f, 0.7f, 0.7f, 0.85f);
    glVertex2f(panelX, panelY);
    glVertex2f(panelX + panelWidth, panelY);
    glVertex2f(panelX + panelWidth, panelY + panelHeight);
    glVertex2f(panelX, panelY + panelHeight);
    glEnd();

    
    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);  
    glVertex2f(panelX - 2, panelY - 2);
    glVertex2f(panelX + panelWidth + 2, panelY - 2);
    glVertex2f(panelX + panelWidth + 2, panelY + panelHeight + 2);
    glVertex2f(panelX - 2, panelY + panelHeight + 2);
    glEnd();

    glDisable(GL_BLEND);

    
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };

    
    TextTextureMenu titleTex = createTextTexture("GAME OVER", font, red);
    renderTexture2D(titleTex, SCREEN_WIDTH/2 - titleTex.width/2, panelY + 50, SCREEN_WIDTH, SCREEN_HEIGHT);
    glDeleteTextures(1, &titleTex.textureId);

   
    std::string scoreText = "Puntuación: " + std::to_string(finalScore);
    TextTextureMenu scoreTex = createTextTexture(scoreText, font, yellow);
    renderTexture2D(scoreTex, SCREEN_WIDTH/2 - scoreTex.width/2, panelY + 150, SCREEN_WIDTH, SCREEN_HEIGHT);
    glDeleteTextures(1, &scoreTex.textureId);

    
    int yPos = panelY + 250;
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        SDL_Color color = (i == selectedOption) ? yellow : white;
        TextTextureMenu optionTex = createTextTexture(menuOptions[i], font, color);
        renderTexture2D(optionTex, SCREEN_WIDTH/2 - optionTex.width/2, yPos, SCREEN_WIDTH, SCREEN_HEIGHT);
        glDeleteTextures(1, &optionTex.textureId);
        yPos += 40;
    }

   
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
    glEnable(GL_LIGHTING);
}

void GameOverMenu::handleInput(SDL_Event& event) {
    Menu::handleInput(event);
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
        executeSelectedOption();
    }
}

void GameOverMenu::executeSelectedOption() {
    switch (selectedOption) {
        case 0: 
            setActive(false); 
            
            break;
        case 1: 
            setActive(false); 
           
            break;
    }
} 