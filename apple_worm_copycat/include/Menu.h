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
private:
    TTF_Font* font;        // Fuente para renderizar el texto del menú
    bool isActive;         // Indica si el menú está activo y visible
    int selectedOption;    // Índice de la opción seleccionada actualmente
    std::vector<std::string> menuOptions;  // Lista de opciones del menú (Jugar, Salir)
    std::vector<std::string> controls;     // Lista de controles a mostrar
    GLuint backgroundTexture;  // Textura de fondo
    bool loadBackgroundTexture(const char* path);  // Función para cargar la textura de fondo

public:
    Menu(TTF_Font* font);  // Constructor que recibe la fuente para el texto
    ~Menu();
    void render();         // Renderiza el menú en pantalla
    void handleInput(SDL_Event& event);  // Maneja la entrada del usuario
    bool isMenuActive() const;  // Retorna si el menú está activo
    void setActive(bool active);  // Activa/desactiva el menú
    int getSelectedOption() const;  // Retorna la opción seleccionada

    // Declaraciones de funciones para renderizar texto
    TextTextureMenu createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color);
    void renderTexture2D(const TextTextureMenu& textTex, int x, int y, int screenWidth, int screenHeight);
    void renderBackground();  // Nueva función para renderizar el fondo
}; 