#pragma once
#include <vector>
#include "Entity.h"
#include "Worm.h"
#include "Apple.h"
#include "Objective.h"

class Game;  // Forward declaration

class Level {
private:
    int levelNumber;
    Worm* worm;
    Apple* apple;
    Objective* objective;
    std::vector<Entity*> entities;
    
    // Posiciones iniciales
    float wormStartX, wormStartY, wormStartZ;
    float appleStartX, appleStartY, appleStartZ;
    float objectiveStartX, objectiveStartY, objectiveStartZ;
    
    Game* game;  // Referencia al juego
    int initialWormLength;  // Longitud inicial del gusano
    
    void createWalls();
    void createWallRow(int start, int end, float y, float z);
    void createWallColumn(float x, float startY, int height);
    void createSingleWall(float x, float y);
    void initialize();

public:
    Level(int levelNumber, Game* game = nullptr);  // Constructor modificado
    ~Level();
    
    void render(bool texture);
    void resetEntities();
    
    std::vector<Entity*>& getEntities();
    Worm* getWorm() const;
    Apple* getApple() const;
    Objective* getObjective() const;
    bool isObjectiveReached() const;
}; 