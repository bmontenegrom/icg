#include "Level.h"
#include "Wall.h"
#include "Worm.h"
#include "WormHead.h"
#include "Apple.h"
#include "Objective.h"
#include "Entity.h"
#include "Game.h"

#include <iostream>

Level::Level(int levelNumber, Game* game) : levelNumber(levelNumber), worm(nullptr), apple(nullptr), objective(nullptr), game(game) {
    // Define posiciones iniciales según el nivel
    switch(levelNumber) {
        case 0:
            wormStartX = 0.1f; wormStartY = 0.125f; wormStartZ = 0.0f;
            appleStartX = 0.5f; appleStartY = 0.2f; appleStartZ = 0.0f;
            objectiveStartX = 0.95f + 0.095f; objectiveStartY = -0.095f; objectiveStartZ = 0.0f;
            initialWormLength = 3; 
            break;
        case 1:
            wormStartX = 0.2f; wormStartY = 0.125f; wormStartZ = 0.0f;
            appleStartX = 0.7f; appleStartY = 0.3f; appleStartZ = 0.0f;
            objectiveStartX = 1.5f; objectiveStartY = 0.0f; objectiveStartZ = 0.0f;
            initialWormLength = 3;  
            break;
        default:
            wormStartX = 0.1f; wormStartY = 0.125f; wormStartZ = 0.0f;
            appleStartX = 0.5f; appleStartY = 0.2f; appleStartZ = 0.0f;
            objectiveStartX = 1.1f; objectiveStartY = -0.1f; objectiveStartZ = 0.0f;
            initialWormLength = 3;  
            break;
    }
    initialize();
}

Level::~Level() {
    for (Entity* entity : entities) {
        delete entity;
    }
    entities.clear();
    if (worm) { delete worm; worm = nullptr; }
    if (apple) { delete apple; apple = nullptr; }
    
    objective = nullptr;
}

void Level::initialize() {
    // Crear el gusano en la posición inicial del nivel (sobre el suelo)
    worm = new Worm(wormStartX, wormStartY, wormStartZ, game);
    worm->reset(wormStartX, wormStartY, wormStartZ, initialWormLength);  // Usar la longitud inicial
    
    // Crear la manzana con el mismo tamaño que el gusano
    apple = new Apple(appleStartX, appleStartY, appleStartZ, 0.095f, 0.095f, 0.095f);
    
    // Crear el objetivo con tamaño pequeño para evitar colisiones invisibles
    objective = new Objective(objectiveStartX, objectiveStartY, objectiveStartZ, 0.095f, 0.095f, 0.095f);
    entities.push_back(objective);
    
    // Crear las paredes según el nivel
    createWalls();
}

void Level::render(bool texture) {
    for (Entity* entity : entities) {
        entity->render(texture);
    }
    worm->render(texture);
    if (apple != nullptr && !apple->eaten()) {
        apple->render(texture);
    }
}

std::vector<Entity*>& Level::getEntities() {
    return entities;
}

Worm* Level::getWorm() const {
    return worm;
}

Apple* Level::getApple() const {
    return apple;
}

Objective* Level::getObjective() const {
    return objective;
}

bool Level::isObjectiveReached() const {
    if (!worm || !objective) return false;
    // Suponiendo que el primer segmento del gusano es la cabeza
    return worm->getHead()->isColliding(*objective);
}

void Level::createWalls() {
    switch(levelNumber) {
        case 0:
            // Nivel actual (nivel 1)
            createWallRow(0, 5, 0.0f, 0.0f);
            createWallRow(8, 12, 0.0f, 0.0f);
            createWallColumn(0.3f, 0.2f, 3);
            createSingleWall(0.7f, 0.3f);
            createWallRow(6, 8, 0.5f, 0.0f);
            break;
        case 1:
            // Nuevo nivel (nivel 2)
            // Plataforma inicial
            createWallRow(0, 4, 0.0f, 0.0f);
            
            // Plataforma elevada
            createWallRow(6, 10, 0.3f, 0.0f);
            
            // Plataforma final
            createWallRow(12, 16, 0.0f, 0.0f);
            
            // Columnas de obstáculos
            createWallColumn(0.5f, 0.1f, 2);
            createWallColumn(0.8f, 0.2f, 3);
            
            // Plataforma intermedia
            createWallRow(4, 6, 0.2f, 0.0f);
            
            // Obstáculos adicionales
            createSingleWall(0.7f, 0.4f);
            createSingleWall(1.1f, 0.3f);
            createSingleWall(1.3f, 0.2f);
            
            // Actualizar posición del objetivo para el nuevo nivel
            objective->setPosition(1.5f, 0.0f, 0.0f);
            break;
        default:
            // Línea eliminada: std::cerr << "Nivel no definido: " << levelNumber << std::endl;
            break;
    }
}

void Level::createWallRow(int start, int end, float y, float z) {
    for(int i = start; i < end; i++) {
        Wall* wall = new Wall(0.095f * i, y, z, 0.095f, 0.095f, 0.095f);
        entities.push_back(wall);
    }
}

void Level::createWallColumn(float x, float startY, int height) {
    for(int i = 0; i < height; i++) {
        Wall* wall = new Wall(x, startY + (0.095f * i), 0.0f, 0.095f, 0.095f, 0.095f);
        entities.push_back(wall);
    }
}

void Level::createSingleWall(float x, float y) {
    Wall* wall = new Wall(x, y, 0.0f, 0.095f, 0.095f, 0.095f);
    entities.push_back(wall);
}

void Level::resetEntities() {
    if (worm) worm->reset(wormStartX, wormStartY, wormStartZ, initialWormLength);  // Usar la longitud inicial
    if (apple) apple->reset(appleStartX, appleStartY, appleStartZ);
    if (objective) objective->setPosition(objectiveStartX, objectiveStartY, objectiveStartZ);
} 