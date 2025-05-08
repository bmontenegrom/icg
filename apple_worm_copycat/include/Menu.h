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

// Struct para almacenar la textura de texto
struct TextTextureMenu {
    GLuint textureId;
    int width;
    int height;
};

class Menu {
protected:
    TTF_Font* font;        // Fuente para renderizar el texto del menú
    bool isActive;         // Indica si el menú está activo y visible
    int selectedOption;    // Índice de la opción seleccionada actualmente
    std::vector<std::string> menuOptions;  // Lista de opciones del menú
    GLuint backgroundTexture;  // Textura de fondo
    bool loadBackgroundTexture(const char* path);  // Función para cargar la textura de fondo

    // Funciones protegidas para renderizar texto
    TextTextureMenu createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color);
    void renderTexture2D(const TextTextureMenu& textTex, int x, int y, int screenWidth, int screenHeight);
    void renderBackground();  // Función para renderizar el fondo

public:
    Menu(TTF_Font* font);  // Constructor que recibe la fuente para el texto
    virtual ~Menu();
    virtual void render() = 0;  // Función virtual pura para renderizar el menú
    virtual void handleInput(SDL_Event& event);  // Función virtual para manejar la entrada
    bool isMenuActive() const;  // Retorna si el menú está activo
    void setActive(bool active);  // Activa/desactiva el menú
    int getSelectedOption() const;  // Retorna la opción seleccionada
    virtual void executeSelectedOption() = 0;  // Función virtual pura para ejecutar la opción seleccionada
}; 