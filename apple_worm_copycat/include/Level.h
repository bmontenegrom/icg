#pragma once
#include <vector>
#include "Entity.h"
#include "Worm.h"
#include "Apple.h"
#include "Objective.h"
#include "XMLParser.h"

class Game;

class Level {
private:
    int levelNumber;
    Worm* worm;
    std::vector<Apple*> apples;
    Objective* objective;
    std::vector<Entity*> entities;
    
    // Posiciones iniciales
    float wormStartX, wormStartY, wormStartZ;
    float appleStartX, appleStartY, appleStartZ;
    float objectiveStartX, objectiveStartY, objectiveStartZ;
    
    Game* game;
    int initialWormLength;
    
    void createWallRow(int start, int end, float y, float z);
    void createWallColumn(float x, float startY, int height);
    void createSingleWall(float x, float y);
    void initialize();
    
    // Nuevos métodos para cargar XML
    bool loadFromXML(const std::string& filename);
    void parseWormNode(XMLNode* wormNode);
    void parseAppleNode(XMLNode* appleNode);
    void parseObjectiveNode(XMLNode* objectiveNode);
    void parseBlocksNode(XMLNode* blocksNode);

public:
    Level(int levelNumber, Game* game = nullptr);
    ~Level();
    
    void render();
    void resetEntities();
    
    std::vector<Entity*>& getEntities();
    Worm* getWorm() const;
    std::vector<Apple*>& getApples();
    Objective* getObjective() const;
    bool isObjectiveReached() const;
}; 