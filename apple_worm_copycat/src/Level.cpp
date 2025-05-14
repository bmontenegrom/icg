#include "Level.h"
#include "Wall.h"
#include "Worm.h"
#include "WormHead.h"
#include "Apple.h"
#include "Objective.h"
#include "Entity.h"
#include "Game.h"
#include <iostream>

Level::Level(int levelNumber, Game* game) : levelNumber(levelNumber), worm(nullptr), objective(nullptr), game(game) {
    // Cargar el nivel desde XML
    std::string filename = "assets/levels/level" + std::to_string(levelNumber) + ".xml";
    if (!loadFromXML(filename)) {
        std::cerr << "Error al cargar el nivel " << levelNumber << " desde XML" << std::endl;
        // Usar valores por defecto si falla la carga
        wormStartX = 0.1f; wormStartY = 0.125f; wormStartZ = 0.0f;
        // appleStartX = 0.5f; appleStartY = 0.2f; appleStartZ = 0.0f;
        objectiveStartX = 1.045f; objectiveStartY = -0.097f; objectiveStartZ = 0.0f;
        initialWormLength = 3;
    }
    initialize();
}

Level::~Level() {
    for (Entity* entity : entities) {
        delete entity;
    }
    entities.clear();
    if (worm) { delete worm; worm = nullptr; }
    for (Apple* apple : apples) { delete apple; }
    apples.clear();
    objective = nullptr;
}

void Level::initialize() {
    // Crear el gusano en la posición inicial del nivel (sobre el suelo)
    worm = new Worm(wormStartX, wormStartY, wormStartZ, game);
    worm->reset(wormStartX, wormStartY, wormStartZ, initialWormLength);
    
    // Crear todas las manzanas
    for (Apple* apple : apples) {
        // Si quieres que las manzanas sean entidades, puedes agregarlas a entities
        entities.push_back(apple);
    }
    
    // Crear el objetivo con tamaño pequeño para evitar colisiones invisibles
    objective = new Objective(objectiveStartX, objectiveStartY, objectiveStartZ, 0.095f, 0.095f, 0.095f);
    entities.push_back(objective);
}

bool Level::loadFromXML(const std::string& filename) {
    XMLNode* root = XMLParser::parseFile(filename);
    if (!root) {
        return false;
    }

    XMLNode* levelsNode = root->getChild("levels");
    if (!levelsNode) {
        delete root;
        return false;
    }

    XMLNode* levelNode = levelsNode->getChild("level");
    if (!levelNode) {
        delete root;
        return false;
    }

    // Parsear cada sección del nivel
    parseWormNode(levelNode->getChild("worm"));
    parseAppleNode(levelNode);
    parseObjectiveNode(levelNode->getChild("objective"));
    parseBlocksNode(levelNode->getChild("blocks"));

    delete root;
    return true;
}

void Level::parseWormNode(XMLNode* wormNode) {
    if (!wormNode) return;
    
    XMLNode* posNode = wormNode->getChild("position");
    if (posNode) {
        wormStartX = std::stof(posNode->getAttribute("x"));
        wormStartY = std::stof(posNode->getAttribute("y"));
        wormStartZ = std::stof(posNode->getAttribute("z"));
    }
    
    XMLNode* lengthNode = wormNode->getChild("initialLength");
    if (lengthNode) {
        initialWormLength = std::stoi(lengthNode->value);
    }
}

void Level::parseAppleNode(XMLNode* levelNode) {
    apples.clear();
    if (!levelNode) return;
    for (XMLNode* child : levelNode->children) {
        if (child->name == "apple") {
            XMLNode* posNode = child->getChild("position");
            if (posNode) {
                float x = std::stof(posNode->getAttribute("x"));
                float y = std::stof(posNode->getAttribute("y"));
                float z = std::stof(posNode->getAttribute("z"));
                apples.push_back(new Apple(x, y, z, 0.095f, 0.095f, 0.095f));
            }
        }
    }
}

void Level::parseObjectiveNode(XMLNode* objectiveNode) {
    if (!objectiveNode) return;
    
    XMLNode* posNode = objectiveNode->getChild("position");
    if (posNode) {
        objectiveStartX = std::stof(posNode->getAttribute("x"));
        objectiveStartY = std::stof(posNode->getAttribute("y"));
        objectiveStartZ = std::stof(posNode->getAttribute("z"));
    }
}

void Level::parseBlocksNode(XMLNode* blocksNode) {
    if (!blocksNode) return;
    
    // Limpiar bloques existentes
    for (Entity* entity : entities) {
        delete entity;
    }
    entities.clear();
    
    // Parsear filas
    for (auto rowNode : blocksNode->children) {
        if (rowNode->name == "row") {
            int start = std::stoi(rowNode->getAttribute("start"));
            int end = std::stoi(rowNode->getAttribute("end"));
            float y = std::stof(rowNode->getAttribute("y"));
            float z = std::stof(rowNode->getAttribute("z"));
            createWallRow(start, end, y, z);
        }
    }
    
    // Parsear columnas
    for (auto colNode : blocksNode->children) {
        if (colNode->name == "column") {
            float x = std::stof(colNode->getAttribute("x"));
            float startY = std::stof(colNode->getAttribute("startY"));
            int height = std::stoi(colNode->getAttribute("height"));
            createWallColumn(x, startY, height);
        }
    }
    
    // Parsear bloques individuales
    for (auto singleNode : blocksNode->children) {
        if (singleNode->name == "single") {
            float x = std::stof(singleNode->getAttribute("x"));
            float y = std::stof(singleNode->getAttribute("y"));
            createSingleWall(x, y);
        }
    }
}

void Level::render() {
    for (Entity* entity : entities) {
        entity->render();
    }
    worm->render();
    for (Apple* apple : apples) {
        if (!apple->eaten()) {
            apple->render();
        }
    }
}

void Level::resetEntities() {
    if (worm) worm->reset(wormStartX, wormStartY, wormStartZ, initialWormLength);
    for (Apple* apple : apples) {
        apple->reset(apple->getX(), apple->getY(), apple->getZ());
    }
    if (objective) objective->setPosition(objectiveStartX, objectiveStartY, objectiveStartZ);
}

std::vector<Entity*>& Level::getEntities() {
    return entities;
}

Worm* Level::getWorm() const {
    return worm;
}

Objective* Level::getObjective() const {
    return objective;
}

bool Level::isObjectiveReached() const {
    if (!worm || !objective) return false;
    
    WormHead* head = worm->getHead();
    if (!head) return false;
    
    float distanceX = std::abs(head->getX() - objective->getX());
    float distanceY = std::abs(head->getY() - objective->getY());
    
    return distanceX < 0.1f && distanceY < 0.1f;
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

std::vector<Apple*>& Level::getApples() {
    return apples;
} 