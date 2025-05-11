#include "MainMenu.h"

MainMenu::MainMenu(TTF_Font* font) : Menu(font) {
    // Inicializa las opciones del menú
    menuOptions = {"Jugar", "Salir"};
    // Inicializa la lista de controles a mostrar
    controls = {
        "Flechas: Mover el gusano",
        "P: Pausar/Reanudar",
        "W: Activar/Desactivar wireframe",
        "V: Cambiar cámara",
        "L: Cambiar posición de la luz",
        "K: Cambiar color de la luz",
        "S: Facetado Interpolado",
        "Q: Salir del juego"
    };
}

MainMenu::~MainMenu() {
}

void MainMenu::render() {
    if (!isActive) return;

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

    // Panel principal con transparencia
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
    glColor4f(0.7f, 0.7f, 0.7f, 0.85f);
    glVertex2f(panelX, panelY);
    glVertex2f(panelX + panelWidth, panelY);
    glVertex2f(panelX + panelWidth, panelY + panelHeight);
    glVertex2f(panelX, panelY + panelHeight);
    glEnd();

    // Borde brillante
    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.6f, 0.2f, 0.5f);
    glVertex2f(panelX - 2, panelY - 2);
    glVertex2f(panelX + panelWidth + 2, panelY - 2);
    glVertex2f(panelX + panelWidth + 2, panelY + panelHeight + 2);
    glVertex2f(panelX - 2, panelY + panelHeight + 2);
    glEnd();

    glDisable(GL_BLEND);

    // Definir colores para el texto
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };
    SDL_Color cyan = { 80, 160, 255, 255 };

    // Renderizar el título del juego
    TextTextureMenu titleTex = createTextTexture("Apple Worm", font, yellow);
    renderTexture2D(titleTex, SCREEN_WIDTH/2 - titleTex.width/2, panelY + 50, SCREEN_WIDTH, SCREEN_HEIGHT);
    glDeleteTextures(1, &titleTex.textureId);

    // Renderizar la lista de controles
    int yPos = panelY + 100;
    for (const auto& control : controls) {
        TextTextureMenu controlTex = createTextTexture(control, font, cyan);
        renderTexture2D(controlTex, SCREEN_WIDTH/2 - controlTex.width/2, yPos, SCREEN_WIDTH, SCREEN_HEIGHT);
        glDeleteTextures(1, &controlTex.textureId);
        yPos += 30;
    }

    // Renderizar las opciones del menú
    yPos = panelY + 350;
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        SDL_Color color = (i == selectedOption) ? yellow : white;
        TextTextureMenu optionTex = createTextTexture(menuOptions[i], font, color);
        renderTexture2D(optionTex, SCREEN_WIDTH/2 - optionTex.width/2, yPos, SCREEN_WIDTH, SCREEN_HEIGHT);
        glDeleteTextures(1, &optionTex.textureId);
        yPos += 40;
    }

    // Restaurar el modo de renderizado original
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
    glEnable(GL_LIGHTING);
}

void MainMenu::handleInput(SDL_Event& event) {
    Menu::handleInput(event);
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
        executeSelectedOption();
    }
}

void MainMenu::executeSelectedOption() {
    switch (selectedOption) {
        case 0: // Jugar
            setActive(false); // Iniciar juego
            break;
        case 1: // Salir
            setActive(false); // Aquí deberías marcar el juego para salir, por ejemplo con un callback o bandera externa
            // Ejemplo: if (onExit) onExit();
            break;
    }
} 