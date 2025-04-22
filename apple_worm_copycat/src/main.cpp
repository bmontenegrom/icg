#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Snake.h"
#include "Apple.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Apple Worm",
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         WINDOW_WIDTH,
                                         WINDOW_HEIGHT,
                                         SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Create SDL renderer for text
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("../assets/font.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! SDL_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Set up OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize game objects
    Snake snake(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    Apple apple;
    apple.respawn(snake);

    // Game variables
    int score = 0;
    bool gameOver = false;
    Uint32 lastUpdate = SDL_GetTicks();
    const Uint32 updateInterval = 100; // milliseconds

    // Main game loop
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                snake.changeDirection(event.key.keysym.sym);
            }
        }

        // Game update
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdate >= updateInterval && !gameOver) {
            snake.move();

            // Check for apple collision
            Position head = snake.getHead();
            Position applePos = apple.getPosition();
            if (head.x == applePos.x && head.y == applePos.y) {
                snake.grow();
                apple.respawn(snake);
                score++;
            }

            // Check for wall collision
            if (head.x < 0 || head.x >= WINDOW_WIDTH || head.y < 0 || head.y >= WINDOW_HEIGHT) {
                gameOver = true;
            }

            // Check for self collision
            for (size_t i = 1; i < snake.getBody().size(); i++) {
                if (head.x == snake.getBody()[i].x && head.y == snake.getBody()[i].y) {
                    gameOver = true;
                    break;
                }
            }

            lastUpdate = currentTime;
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw game objects
        snake.draw();
        apple.draw();

        // Switch to SDL rendering for text
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw score
        SDL_Color white = {255, 255, 255, 255};
        renderText(renderer, font, "Score: " + std::to_string(score), 10, 10, white);

        if (gameOver) {
            SDL_Color red = {255, 0, 0, 255};
            renderText(renderer, font, "Game Over!", WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2, red);
        }

        // Swap buffers
        SDL_GL_SwapWindow(window);
        SDL_RenderPresent(renderer);
    }

    // Clean up
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
