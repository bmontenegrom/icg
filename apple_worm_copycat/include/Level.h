#pragma once
#include "Entity.h"
#include "Worm.h"
#include "Apple.h"
#include "Objective.h"
#include <vector>

class Level {
private:
    std::vector<Entity*> entities;
    Worm* worm;
    Apple* apple;
    Objective* objective;
    int levelNumber;
    // Posiciones iniciales
    
    
public:
    Level(int levelNumber);
    ~Level();
    
	// Posiciones iniciales
    float wormStartX, wormStartY, wormStartZ;
    float appleStartX, appleStartY, appleStartZ;
    float objectiveStartX, objectiveStartY, objectiveStartZ;

    void initialize();
    void render();
    std::vector<Entity*>& getEntities();
    Worm* getWorm() const;
    Apple* getApple() const;
    Objective* getObjective() const;
    bool isObjectiveReached() const;
    void resetEntities();
    
private:
    void createWalls();
    void createWallRow(int start, int end, float y, float z);
    void createWallColumn(float x, float startY, int height);
    void createSingleWall(float x, float y);
}; 