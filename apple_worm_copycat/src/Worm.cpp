#include "Worm.h"
#include "WormHead.h"
#include "WormTail.h"
#include "WormBody.h"
#include "Apple.h"
#include "Game.h"
#include <iostream>

Worm::Worm(double x, double y, double z, Game* game) : Entity(x, y, z), game(game) {
	double segment_size = 0.095f;  // 5% más pequeño que los bloques
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
	this->speed = 0.095f;  // Velocidad ajustada al nuevo tamaño
	
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

void Worm::move(Direction newDirection, const std::vector<Entity*> &walls, std::vector<Apple*>& apples, float timeStep)
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

	// Ajustar la velocidad para que coincida con el nuevo tamaño
	float distance = 0.095f;  // Mismo tamaño que los segmentos del gusano
	
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

	// Verificar colisiones con el cuerpo
	bool bodyCollision = false;
	for (int i = 1; i < body.size(); ++i) {
		if (this->head->isColliding(body[i])) {
			bodyCollision = true;
			break;
		}
	}

	// Verificar colisiones con las paredes
	bool wallCollision = false;
	for (auto wall : walls) {
		if (this->head->isColliding(wall)) {
			wallCollision = true;
			break;
		}
	}

	// Si hay colisión, revertir el movimiento
	if (bodyCollision || wallCollision) {
		this->head->setPosition(oldX, oldY, oldZ);
		this->head->setDirection(oldDirection);
		return;
	}

	// Checkea si el gusano choca con la manzana
	for (Apple* apple : apples) {
		if (apple != nullptr && !apple->eaten()) {
			WormHead* head = this->head;
			double headX = head->getX();
			double headY = head->getY();
			double appleX = apple->getX();
			double appleY = apple->getY();

			// Calcular la distancia entre la cabeza y la manzana
			double distanceX = std::abs(headX - appleX);
			double distanceY = std::abs(headY - appleY);
			std::cout << "[DEBUG] Distancia a manzana: X=" << distanceX << ", Y=" << distanceY << std::endl;

			// Usar un margen más permisivo para la colisión
			const double COLLISION_MARGIN = 0.1f;

			// Verificar si la cabeza está lo suficientemente cerca de la manzana
			if (distanceX < COLLISION_MARGIN && distanceY < COLLISION_MARGIN) {
				std::cout << "[DEBUG] ¡Colisión con manzana detectada!" << std::endl;
				this->length++;
				double bodyX = this->body[body.size() - 2]->getPrevX();
				double bodyY = this->body[body.size() - 2]->getPrevY();
				double bodyZ = this->body[body.size() - 2]->getPrevZ();
				WormBody* segment = new WormBody(bodyX, bodyY, bodyZ, this->head->getWidth(), this->head->getHeight(), this->head->getDepth(), this->head->getDirection());
				segment->updatePreviousPosition();
				this->body.insert(this->body.end() - 1, segment);
				apple->setEaten(true);
				std::cout << "[DEBUG] Manzana marcada como comida." << std::endl;

				// Incrementar el puntaje
				if (game != nullptr) {
					game->setScore(game->getScore() + 10);  // Incrementar el puntaje en 10 puntos
				}
			}
		}
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

bool Worm::isOnGround(const std::vector<Entity*> &walls) const {
    // Recorre todos los segmentos del gusano
    for (Entity* segment : body) {
        for (Entity* wall : walls) {
            // Condición más estricta: el centro del segmento debe estar dentro del ancho del bloque
            if (std::abs(segment->getY() - (wall->getY() + wall->getHeight())) < 0.005f &&
                segment->getX() > wall->getX() - wall->getWidth()/2 &&
                segment->getX() < wall->getX() + wall->getWidth()/2) {
                return true;
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
    double segment_size = 0.095f;  // 5% más pequeño que los bloques
    Direction initialDirection = Direction::RIGHT;
    
    // Crear la cabeza
    this->head = new WormHead(x, y, z, segment_size, segment_size, segment_size, initialDirection);
    this->body.push_back(head);
    
    // Actualizar propiedades
    this->length = length;
    this->isFalling = false;
    this->verticalVelocity = 0.0f;
    this->fallStartY = y;
    this->speed = 0.095f;  // Velocidad ajustada al nuevo tamaño
    
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

