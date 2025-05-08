#pragma once

#include <vector>
#include <SDL.h>
#include "Constants.h" // Usar la definición de GameState de aquí
#include "Display.h"
#include "Timer.h"
#include "Camera.h"
#include "Hud.h"
#include "MainMenu.h"
#include "GameOverMenu.h"
#include "Level.h"
#include "Worm.h"
#include "Apple.h"

class Game {
private:
    // Variables de estado del juego
    int score;               // Puntaje actual
    GameState currentState;  // Estado actual del juego
    bool isRunning;         // Indica si el juego está en ejecución
    bool isPaused;          // Indica si el juego está en pausa
    bool hasStartedPlaying; // Indica si el jugador ha comenzado a jugar

    // Componentes principales
    Display* display;        // Manejo de la ventana y renderizado
    Timer* timer;           // Control de tiempo del juego
    Camera* camera;         // Control de la cámara
    Hud* hud;              // Interfaz de usuario durante el juego
    MainMenu* mainMenu;     // Menú principal
    GameOverMenu* gameOverMenu; // Menú de game over

    // Niveles del juego
    std::vector<Level*> levels;  // Vector de niveles disponibles
    int currentLevel;           // Nivel actual

    // Métodos privados
    void handleKeyPress(SDL_Keycode key, Worm* worm, Apple* apple, float timeStep, bool& wireframe, bool& texture);
    void updateCamera(Worm* worm);
    void loadLevel(int levelNumber);
    void nextLevel();
    void resetGame();
    void changeState(GameState newState);  // Método para cambiar el estado del juego

public:
    // Constructor y destructor
    Game();
    ~Game();

    // Métodos principales
    void run();  // Bucle principal del juego

    // Getters y setters
    int getScore() const;
    void setScore(int newScore);
}; 