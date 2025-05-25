#include "Wall.h"
#include "Objective.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <WormHead.h>
#include "Constants.h"
#include <Game.h>
#include "Background.h"
#include "WinnerMenu.h"



Game::Game() : score(0), currentLevel(0), isRunning(true), isPaused(false), 
    hasStartedPlaying(false), currentState(MENU), gameSpeedMultiplier(1.0f) 
{
    
    this->display = new Display();
    this->timer = new Timer();
    this->camera = new Camera();
    
    
    if (TTF_Init() != 0) {
        exit(1);
    }

    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
    if (font == nullptr) {
        exit(1);
    }

    
    this->hud = new Hud(font);
    this->mainMenu = new MainMenu(font);
    this->gameOverMenu = new GameOverMenu(font);
    this->winnerMenu = new WinnerMenu(font);

    
    levels.push_back(new Level(0, this)); 
    levels.push_back(new Level(1, this)); 
    levels.push_back(new Level(2, this));

    
    mainMenu->setActive(true);
    gameOverMenu->setActive(false);

    background = new Background();
}


Game::~Game() {
    for (Level* level : levels) {
        delete level;
    }
    delete display;
    delete timer;
    delete camera;
    delete hud;
    delete mainMenu;
    delete gameOverMenu;
    delete winnerMenu;
    delete background;
}


int Game::getScore() const {
    return this->score;
}


void Game::setScore(int newScore) {
    this->score = newScore;
}


void Game::changeState(GameState newState) {
    currentState = newState;
    
    
    switch (currentState) {
        case MENU:
            mainMenu->setActive(true);
            gameOverMenu->setActive(false);
            isPaused = false;
            break;
            
        case PLAYING:
            mainMenu->setActive(false);
            gameOverMenu->setActive(false);
            isPaused = false;
            break;
            
        case GAME_OVER:
            mainMenu->setActive(false);
            gameOverMenu->setActive(true);
            isPaused = true;
            break;
            
        case PAUSED:
            isPaused = true;
            break;
            
        case WINNER:
            mainMenu->setActive(false);
            gameOverMenu->setActive(false);
            winnerMenu->setActive(true);
            isPaused = true;
            break;
    }
}


void Game::resetGame() {
    
    for (int i = 0; i < levels.size(); i++) {
		levels[i]->resetEntities();
    }
    
    setScore(0);
    hasStartedPlaying = false;
    isPaused = false;
    hud->startTime();
    gameOverMenu->setActive(false);
    mainMenu->setActive(false);
    winnerMenu->setActive(false);
    SDL_Event dummy;
    while (SDL_PollEvent(&dummy)) {}
}


void Game::setGameSpeed(float speed) {
    
    if (speed >= 0.25f && speed <= 4.0f) {
        gameSpeedMultiplier = speed;
    }
}


float Game::getGameSpeed() const {
    return gameSpeedMultiplier;
}


void Game::run() {
    
	display->resetPerspective();
    
    
    bool wireframe = false;
    bool texture = true;
	bool smooth = true;

    SDL_Event event;
    this->hud->startTime();
    setScore(0);

   
    while (isRunning) {
        
		display->resetPerspective();
        
        
        float timeStep = (timer->getTicks() / 1000.0f) * gameSpeedMultiplier;
        Level* currentLevelPtr = levels[currentLevel];
        Worm* worm = currentLevelPtr->getWorm();
        std::vector<Apple*>& apples = currentLevelPtr->getApples();
        
        if (apples.empty()) {
			levels[currentLevel]->manzanaComida(true);
        }

        
        switch (currentState) {
            case MENU:
                mainMenu->handleInput(event);
                if (!mainMenu->isMenuActive()) {
                    if (mainMenu->getSelectedOption() == 1) {
                        isRunning = false;  
                    }
                    else if (mainMenu->getSelectedOption() == 0) {
                        setScore(0);
                        hasStartedPlaying = false;
                        changeState(PLAYING);  
                    }
                }
                break;

            case PLAYING:
                if (!isPaused) {
                    
                    worm->updateGravity(currentLevelPtr->getEntities(), timeStep);
                    
                    
                    if(currentLevelPtr->isObjectiveReached()){
                        if (currentLevel + 1 < levels.size()) {
                            nextLevel();
                            this->hud->startTime();
                        } else {
                            winnerMenu->setFinalScore(getScore());
                            winnerMenu->setFinalTime(hud->getTime());
                            winnerMenu->setActive(true);
                            changeState(WINNER);
                        }
                        continue;
                    }

                    
                    if (hasStartedPlaying && 
                        worm->getIsFalling() && 
                        worm->getVerticalVelocity() > 0.5f && 
                        worm->getHead()->getY() < -1.0f &&    
                        worm->getHead()->getY() < worm->getFallStartY() - 1.5f) {
                        gameOverMenu->setFinalScore(getScore());
                        changeState(GAME_OVER);
                    }
                }
                break;

            case GAME_OVER:
                gameOverMenu->handleInput(event);
                if (!gameOverMenu->isMenuActive()) {
                    if (gameOverMenu->getSelectedOption() == 0) {
                        
						this->currentLevel = 0; 
                        resetGame();
                        changeState(PLAYING);
                        continue; 
                    } else {
                        
                        resetGame(); 
                        changeState(MENU);
                        continue; 
                    }
                }
                break;

            case WINNER:
                winnerMenu->render();
                break;
        }

        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            else if (currentState == MENU) {
                mainMenu->handleInput(event);
            }
            else if (currentState == GAME_OVER) {
                gameOverMenu->handleInput(event);
            }
            else if (currentState == WINNER) {
                winnerMenu->handleInput(event);
            }
            else if (event.type == SDL_KEYDOWN) {
                handleKeyPress(event.key.keysym.sym, worm, apples, timeStep, wireframe, texture, smooth);
            }
            else if (event.type == SDL_MOUSEMOTION && this->camera->getCameraMode() == CameraMode::FREE_CAMERA) {
                int mouseX, mouseY;
                SDL_GetRelativeMouseState(&mouseX, &mouseY);
                this->camera->updateMouseMovement(mouseX, mouseY);
            }

        }

       
        updateCamera(worm);

       
        this->timer->start();

        

        background->render();

        display->resetPerspective();
        camera->applyView();

        
        switch (currentState) {
            case MENU:
                mainMenu->render();
                break;
            case GAME_OVER:
                gameOverMenu->render();
                camera->resetToDefault();
                break;
            case PLAYING:
                currentLevelPtr->render(texture);
                hud->render(getScore(), this->hud->getTime(), getGameSpeed());
                break;
            case WINNER:
                winnerMenu->render();
                break;
        }

        
		display->swapWindow();


        
        if (timeStep * 1000 < 17) {
			SDL_Delay(17 - timeStep * 1000);
        }
        
        if (currentState == WINNER && !winnerMenu->isMenuActive()) {
            if (winnerMenu->getSelectedOption() == 0) {
                setScore(0);
                hud->startTime();
                currentLevel = 0;
                resetGame();
                changeState(PLAYING);
                continue;
            } else {
                setScore(0);
                hud->startTime();
                currentLevel = 0;
                resetGame();
                changeState(MENU);
                continue;
            }
        }
    }
}

void Game::handleKeyPress(SDL_Keycode key, Worm* worm, std::vector<Apple*>& apples, float timeStep, bool& wireframe, bool& texture, bool &smooth) {
    if (worm->getIsFalling()) {
        return;
    }
    switch (key) {
        case SDLK_UP:
            if (!isPaused) { worm->move(UP, levels[currentLevel]->getEntities(), apples, timeStep); hasStartedPlaying = true; }
            break;
        case SDLK_DOWN:
            if (!isPaused) { worm->move(DOWN, levels[currentLevel]->getEntities(), apples, timeStep); hasStartedPlaying = true; }
            break;
        case SDLK_LEFT:
            if (!isPaused) { worm->move(LEFT, levels[currentLevel]->getEntities(), apples, timeStep); hasStartedPlaying = true; }
            break;
        case SDLK_RIGHT:
            if (!isPaused) { worm->move(RIGHT, levels[currentLevel]->getEntities(), apples, timeStep); hasStartedPlaying = true; }
            break;
        case SDLK_p:
            isPaused = !isPaused;
			this->levels[currentLevel]->pauseObjective(isPaused);
            break;
        case SDLK_v:
            this->camera->switchCameraMode();
            SDL_SetRelativeMouseMode(this->camera->getCameraMode() == CameraMode::FREE_CAMERA ? SDL_TRUE : SDL_FALSE);
            break;
        case SDLK_q:
            isRunning = false;
            break;
        case SDLK_w:
            wireframe = !wireframe;
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
            break;
        case SDLK_t:
            texture = !texture;
            break;
		case SDLK_s:
			smooth = !smooth;
            if (smooth) {
                glShadeModel(GL_SMOOTH);
            }
            else {
				glShadeModel(GL_FLAT);
            }
            break;
        case SDLK_l:
            this->display->changeLightPosition();
            break;
        case SDLK_k:
            this->display->changeLightColor();
            break;
        case SDLK_ESCAPE:
            mainMenu->setActive(true);
            break;
        case SDLK_1: 
            setGameSpeed(0.25f);
            break;
        case SDLK_2: 
            setGameSpeed(0.5f);
            break;
        case SDLK_3: 
            setGameSpeed(1.0f);
            break;
        case SDLK_4: 
            setGameSpeed(2.0f);
            break;
        case SDLK_5: 
            setGameSpeed(4.0f);
            break;
    }
}

void Game::updateCamera(Worm* worm) {
    if (camera->getCameraMode() == CameraMode::THIRD_PERSON) {
        Direction dir = worm->getHeadDirection();
        float dirX = 0.0f, dirY = 0.0f, dirZ = 0.0f;
        switch (dir) {
            case UP: dirY = 1.0f; break;
            case DOWN: dirY = -1.0f; break;
            case LEFT: dirX = -1.0f; break;
            case RIGHT: dirX = 1.0f; break;
        }
        this->camera->followTarget(
            worm->getX(),
            worm->getY(),
            worm->getZ());
    }
}

void Game::loadLevel(int levelNumber) {
    if (levelNumber >= 0 && levelNumber < levels.size()) {
        currentLevel = levelNumber;
    }
}

void Game::nextLevel() {
    if (currentLevel + 1 < levels.size()) {
        currentLevel++;
    } else {
       
        winnerMenu->setFinalScore(getScore());
        winnerMenu->setFinalTime(hud->getTime());
        winnerMenu->setActive(true);
        changeState(WINNER);
    }
}