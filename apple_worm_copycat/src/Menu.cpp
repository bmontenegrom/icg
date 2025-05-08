#include "Menu.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <iostream>
#include <string>
#include <SDL_ttf.h>
#include <FreeImage.h>

Menu::Menu(TTF_Font* font) : font(font), isActive(true), selectedOption(0), backgroundTexture(0) {
    // Cargar la textura de fondo
    if (!loadBackgroundTexture("assets/menu_background.png")) {
        // std::cerr << "Error al cargar la textura de fondo" << std::endl;
    }
}

Menu::~Menu() {
    if (backgroundTexture != 0) {
        glDeleteTextures(1, &backgroundTexture);
    }
}

bool Menu::loadBackgroundTexture(const char* path) {
    // Cargar la imagen usando FreeImage
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path, 0);
    if (fif == FIF_UNKNOWN) {
        fif = FreeImage_GetFIFFromFilename(path);
    }
    if (fif == FIF_UNKNOWN) {
        // std::cerr << "Formato de imagen desconocido" << std::endl;
        return false;
    }

    FIBITMAP* dib = FreeImage_Load(fif, path);
    if (!dib) {
        // std::cerr << "Error al cargar la imagen" << std::endl;
        return false;
    }

    // Convertir a 32 bits si es necesario
    FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);
    FreeImage_Unload(dib);
    if (!dib32) {
        // std::cerr << "Error al convertir la imagen a 32 bits" << std::endl;
        return false;
    }

    // Obtener las dimensiones
    int width = FreeImage_GetWidth(dib32);
    int height = FreeImage_GetHeight(dib32);

    // Voltear la imagen verticalmente (OpenGL espera las imágenes al revés)
    FreeImage_FlipVertical(dib32);

    // Obtener los datos de la imagen
    BYTE* bits = FreeImage_GetBits(dib32);

    // Generar y configurar la textura OpenGL
    glGenTextures(1, &backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, bits);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Liberar la memoria
    FreeImage_Unload(dib32);
    return true;
}

void Menu::renderBackground() {
    if (backgroundTexture == 0) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(SCREEN_WIDTH, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(SCREEN_WIDTH, SCREEN_HEIGHT);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0, SCREEN_HEIGHT);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

TextTextureMenu Menu::createTextTexture(const std::string& text, TTF_Font* font, SDL_Color color) {
    TextTextureMenu result;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) {
        // std::cerr << "Error creando surface: " << TTF_GetError() << std::endl;
        result.textureId = 0;
        result.width = 0;
        result.height = 0;
        return result;
    }
    SDL_Surface* formatted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);
    if (!formatted) {
        // std::cerr << "Error al convertir superficie a RGBA32: " << SDL_GetError() << std::endl;
        result.textureId = 0;
        result.width = 0;
        result.height = 0;
        return result;
    }
    result.width = formatted->w;
    result.height = formatted->h;
    glGenTextures(1, &result.textureId);
    glBindTexture(GL_TEXTURE_2D, result.textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        formatted->w, formatted->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, formatted->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(formatted);
    return result;
}

void Menu::renderTexture2D(const TextTextureMenu& textTex, int x, int y, int screenWidth, int screenHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, screenHeight, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textTex.textureId);
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2i(x, y);
    glTexCoord2f(1, 0); glVertex2i(x + textTex.width, y);
    glTexCoord2f(1, 1); glVertex2i(x + textTex.width, y + textTex.height);
    glTexCoord2f(0, 1); glVertex2i(x, y + textTex.height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

<<<<<<< HEAD
=======
void Menu::render() {
    if (!isActive) return;  // No renderizar si el menú no está activo

    // Desactivar iluminación para el menú
    glDisable(GL_LIGHTING);

    // Guardar el modo actual de renderizado
    GLint polygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, polygonMode);

    // Cambiar a modo relleno para el menú
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Renderizar el fondo
    renderBackground();

    // Panel central con efecto de brillo
    float panelWidth = 400;
    float panelHeight = 500;
    float panelX = (SCREEN_WIDTH - panelWidth) / 2;
    float panelY = (SCREEN_HEIGHT - panelHeight) / 2;

    // Panel principal con transparencia (más claro)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Panel principal
    glBegin(GL_QUADS);
    glColor4f(0.7f, 0.7f, 0.7f, 0.85f);  // Gris claro con más opacidad
    glVertex2f(panelX, panelY);
    glVertex2f(panelX + panelWidth, panelY);
    glVertex2f(panelX + panelWidth, panelY + panelHeight);
    glVertex2f(panelX, panelY + panelHeight);
    glEnd();

    // Borde brillante (naranja suave)
    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.6f, 0.2f, 0.5f);  // Naranja suave con más opacidad
    glVertex2f(panelX - 2, panelY - 2);
    glVertex2f(panelX + panelWidth + 2, panelY - 2);
    glVertex2f(panelX + panelWidth + 2, panelY + panelHeight + 2);
    glVertex2f(panelX - 2, panelY + panelHeight + 2);
    glEnd();

    glDisable(GL_BLEND);

    // Definir colores para el texto
    SDL_Color white = { 255, 255, 255, 255 };  // Blanco puro para texto normal
    SDL_Color yellow = { 255, 255, 0, 255 };   // Amarillo puro para opción seleccionada
    SDL_Color cyan = { 80, 160, 255, 255 };    // Azul suave para controles

    // Renderizar el título del juego (sin sombra)
    TextTextureMenu titleTex = createTextTexture("Apple Worm", font, yellow);
    renderTexture2D(titleTex, SCREEN_WIDTH/2 - titleTex.width/2, panelY + 50, SCREEN_WIDTH, SCREEN_HEIGHT);
    glDeleteTextures(1, &titleTex.textureId);

    // Renderizar la lista de controles (sin sombra)
    int yPos = panelY + 100;
    for (const auto& control : controls) {
        TextTextureMenu controlTex = createTextTexture(control, font, cyan);
        renderTexture2D(controlTex, SCREEN_WIDTH/2 - controlTex.width/2, yPos, SCREEN_WIDTH, SCREEN_HEIGHT);
        glDeleteTextures(1, &controlTex.textureId);
        yPos += 30;
    }

    // Renderizar las opciones del menú (sin sombra)
    yPos = panelY + 350;
    for (int i = 0; i < static_cast<int>(menuOptions.size()); i++) {
        SDL_Color color = (i == selectedOption) ? yellow : white;
        TextTextureMenu optionTex = createTextTexture(menuOptions[i], font, color);
        renderTexture2D(optionTex, SCREEN_WIDTH/2 - optionTex.width/2, yPos, SCREEN_WIDTH, SCREEN_HEIGHT);
        glDeleteTextures(1, &optionTex.textureId);
        yPos += 40;
    }

    // Restaurar el modo de renderizado anterior
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);

    // Volver a activar la iluminación
    glEnable(GL_LIGHTING);
}

>>>>>>> 7f7f19f3a3739ae3412822da323d55dae1957cbc
void Menu::handleInput(SDL_Event& event) {
    if (!isActive) return;

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                selectedOption = (selectedOption - 1 + menuOptions.size()) % menuOptions.size();
                break;
            case SDLK_DOWN:
                selectedOption = (selectedOption + 1) % menuOptions.size();
                break;
        }
    }
}

bool Menu::isMenuActive() const {
    return isActive;
}

void Menu::setActive(bool active) {
    isActive = active;
}

int Menu::getSelectedOption() const {
    return selectedOption;
}
