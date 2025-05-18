#include "Worm.h"
#include "WormHead.h"
#include "WormTail.h"
#include "WormBody.h"
#include "Apple.h"
#include "Game.h"
#include <iostream>

Worm::Worm(double x, double y, double z, Game* game) : Entity(x, y, z), game(game) {
	double segment_size = 0.095f;  // Tamaño exacto del bloque
	Direction initialDirection = Direction::RIGHT;
	
	// Inicializar la cabeza
	this->head = new WormHead(x, y, z, segment_size, segment_size, segment_size, initialDirection);
	this->head->updatePreviousPosition();
	this->body = std::vector<Entity*>();
	this->body.push_back(head);
	
	// Inicializar el cuerpo
	this->length = 3;  // Cabeza + cuerpo + cola
	this->isFalling = false;
	this->verticalVelocity = 0.0f;
	this->fallStartY = y;
	this->speed = 0.095f;  // Velocidad igual al tamaño del bloque
	
	// Crear el segmento del cuerpo
	double bodyX = x - segment_size;
	double bodyY = y;
	double bodyZ = z;
	WormBody* segment = new WormBody(bodyX, bodyY, bodyZ, segment_size, segment_size, segment_size, initialDirection);
	segment->updatePreviousPosition();
	this->body.push_back(segment);
	
	// Crear la cola
	this->tail = new WormTail(x - (2 * segment_size), y, z, segment_size, segment_size, segment_size, initialDirection);
	this->tail->updatePreviousPosition();
	this->body.push_back(tail);
}

Worm::~Worm() {
	for (Entity* segment : body) {
		delete segment;
	}
}

void Worm::render(bool texture){
	for (Entity* segment : body) {
		segment->render(texture);
	}
}

void Worm::move(Direction newDirection, std::vector<Entity*>& entities, std::vector<Apple*>& apples, float timeStep)
{
	for (Entity* segment : body) {
		segment->updatePreviousPosition();
	}
	// No permitir movimiento en dirección opuesta
	if (this->head->getDirection() == Direction::UP && newDirection == Direction::DOWN ||
		this->head->getDirection() == Direction::DOWN && newDirection == Direction::UP ||
		this->head->getDirection() == Direction::LEFT && newDirection == Direction::RIGHT ||
		this->head->getDirection() == Direction::RIGHT && newDirection == Direction::LEFT) {
		return;
	}

	double oldX = this->head->getX();
	double oldY = this->head->getY();
	double oldZ = this->head->getZ();
	Direction oldDirection = this->head->getDirection();

	this->head->setDirection(newDirection);

	// Usar una velocidad fija igual al tamaño del bloque
	float distance = 0.095f;  // Tamaño del bloque
	
	// Intentar mover en la nueva dirección
	switch (this->getHeadDirection())
	{
	case UP:
		this->head->setY(this->head->getY() + distance);
		break;
	case DOWN:
		this->head->setY(this->head->getY() - distance);
		break;
	case LEFT:
		this->head->setX(this->head->getX() - distance);
		break;
	case RIGHT:
		this->head->setX(this->head->getX() + distance);
		break;
	}

	// Checkea si el gusano choca con la manzana ANTES de comprobar colisión con paredes
	for (auto it = apples.begin(); it != apples.end();) {
		Apple* apple = *it;
		if (apple != nullptr && !apple->eaten()) {
			if (this->head->isColliding(apple)) {
				// Comer manzana desde cualquier posición
				this->length++;
				double bodyX = this->tail->getX();
				double bodyY = this->tail->getY();
				double bodyZ = this->tail->getZ();
				WormBody* segment = new WormBody(bodyX, bodyY, bodyZ, this->head->getWidth(), this->head->getHeight(), this->head->getDepth(), this->head->getDirection());
				segment->updatePreviousPosition();
				this->body.insert(this->body.end() - 1, segment);
				Entity* penultimo = this->body[this->body.size() - 2];
				double tailPrevX = penultimo->getPrevX();
				double tailPrevY = penultimo->getPrevY();
				double tailPrevZ = penultimo->getPrevZ();
				this->tail->setPosition(tailPrevX, tailPrevY, tailPrevZ);
				this->tail->updatePreviousPosition();
				apple->setEaten(true);
				if (game != nullptr) {
					game->setScore(game->getScore() + 10);
				}
				it = apples.erase(it);
				auto entIt = std::find(entities.begin(), entities.end(), apple);
				if (entIt != entities.end()) {
					entities.erase(entIt);
				}
				delete apple;
				return;
			} else {
				++it;
			}
		} else {
			++it;
		}
	}

	// Verificar colisiones con el cuerpo
	bool bodyCollision = false;
	for (int i = 1; i < body.size(); ++i) {
		double dx = this->head->getX() - body[i]->getX();
		double dy = this->head->getY() - body[i]->getY();
		double dist = std::sqrt(dx*dx + dy*dy);
		double minDist = (this->head->getWidth() + body[i]->getWidth()) / 2.0;
		if (dist < minDist) {
			std::cout << "[DEBUG] Colisión euclidiana con el cuerpo en segmento " << i << ". Distancia: " << dist << " < minDist: " << minDist << std::endl;
			bodyCollision = true;
			break;
		}
	}

	// Verificar colisiones con las paredes
	bool wallCollision = false;
	for (auto wall : entities) {
		if (wall->getType() != EntityType::WALL) continue;
		if (this->head->isColliding(wall)) {
			std::cout << "[DEBUG] Colisión con pared en posición: x=" << wall->getX() << ", y=" << wall->getY() << std::endl;
			wallCollision = true;
			break;
		}
	}

	// Si hay colisión, revertir el movimiento
	if (bodyCollision || wallCollision) {
		std::cout << "[DEBUG] Movimiento revertido. bodyCollision=" << bodyCollision << ", wallCollision=" << wallCollision << std::endl;
		this->head->setPosition(oldX, oldY, oldZ);
		this->head->setDirection(oldDirection);
		return;
	}

	// Actualizar las posiciones de los segmentos del cuerpo
	double prevX = oldX;
	double prevY = oldY;
	double prevZ = oldZ;

	for (int i = 1; i < body.size(); ++i) {
		double currentX = body[i]->getX();
		double currentY = body[i]->getY();
		double currentZ = body[i]->getZ();
		
		body[i]->setPosition(prevX, prevY, prevZ);
		
		prevX = currentX;
		prevY = currentY;
		prevZ = currentZ;
	}

	this->setPosition(this->head->getX(), this->head->getY(), this->head->getZ());
}

void Worm::setSpeed(double speed)
{
	this->speed = speed;
}

int Worm::getLength() const
{
	return this->length;
}

double Worm::getSpeed() const
{
	return this->speed;
}

Direction Worm::getHeadDirection() const
{
	return this->head->getDirection();
}

void Worm::updateGravity(const std::vector<Entity*> &walls, float timeStep) {
    if (!isOnGround(walls)) {
        if (!isFalling) {
            isFalling = true;
            fallStartY = head->getY();
            verticalVelocity = 0.0f;
        }
        
        verticalVelocity += GRAVITY * timeStep;
        // Limitar la velocidad máxima de caída a 0.18f
        const float MAX_FALL_SPEED = 0.9f;
        if (verticalVelocity > MAX_FALL_SPEED) {
            verticalVelocity = MAX_FALL_SPEED;
        }
        float newY = head->getY() - verticalVelocity * timeStep;
        
        for (Entity* segment : body) {
            segment->setY(segment->getY() - verticalVelocity * timeStep);
        }
    } else {
        isFalling = false;
        verticalVelocity = 0.0f;
    }
}

bool Worm::isOnGround(const std::vector<Entity*> &entities) const {
    // Revisar todos los segmentos del cuerpo
    for (Entity* segment : body) {
        for (Entity* entity : entities) {
            // Considerar tanto WALL como APPLE no comida
            if (entity->getType() == EntityType::WALL ||
                (entity->getType() == EntityType::APPLE && !static_cast<Apple*>(entity)->eaten())) {
                const double GROUND_MARGIN = 0.02f;
                bool isOnGround = std::abs(segment->getY() - (entity->getY() + entity->getHeight())) < GROUND_MARGIN &&
                                  segment->getX() > entity->getX() - entity->getWidth()/2 &&
                                  segment->getX() < entity->getX() + entity->getWidth()/2;
                if (isOnGround) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Worm::reset(double x, double y, double z, int length) {
    std::cout << "Resetting worm to position: " << x << ", " << y << ", " << z << " with length: " << length << std::endl;
    
    // Borra el cuerpo anterior
    for (Entity* segment : body) {
        delete segment;
    }
    body.clear();

    // Reconstruye el cuerpo
    double segment_size = 0.095f;  // Tamaño exacto del bloque
    Direction initialDirection = Direction::RIGHT;
    
    // Crear la cabeza
    this->head = new WormHead(x, y, z, segment_size, segment_size, segment_size, initialDirection);
    this->body.push_back(head);
    
    // Actualizar propiedades
    this->length = length;
    this->isFalling = false;
    this->verticalVelocity = 0.0f;
    this->fallStartY = y;
    this->speed = 0.095f;  // Velocidad igual al tamaño del bloque
    
    // Crear los segmentos del cuerpo
    for (int i = 1; i < length - 1; i++) {
        double bodyX = x - (i * segment_size);
        double bodyY = y;
        double bodyZ = z;
        WormBody* segment = new WormBody(bodyX, bodyY, bodyZ, segment_size, segment_size, segment_size, initialDirection);
        this->body.push_back(segment);
    }
    
    // Crear la cola
    this->tail = new WormTail(x - ((length - 1) * segment_size), y, z, segment_size, segment_size, segment_size, initialDirection);
	this->body.push_back(tail);
	for (Entity* segment : body) {
		segment->updatePreviousPosition();
	}
}

bool Worm::getIsFalling() const {
    return isFalling;
}

float Worm::getVerticalVelocity() const {
    return verticalVelocity;
}

float Worm::getFallStartY() const {
    return fallStartY;
}

void Worm::setGame(Game* newGame) {
    game = newGame;
}

