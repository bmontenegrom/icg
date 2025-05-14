#pragma once
#include "Entity.h"
#include <vector>
#include "Constants.h"

class WormHead;
class WormBody;
class WormTail;
class Apple;
class Game;  // Forward declaration

class Worm : public Entity {
private:
	WormHead* head;
	WormTail* tail;
	std::vector<Entity*> body;
	int length;
	double speed;
	// Indica si el gusano está actualmente en estado de caída
	bool isFalling;
	// Velocidad vertical actual del gusano (positiva hacia abajo)
	float verticalVelocity;
	// Posición Y desde donde comenzó la caída
	float fallStartY;
	Game* game;  // Referencia al juego

public:
	Worm(double x, double y, double z, Game* game = nullptr);  // Constructor modificado
	~Worm();
	void render(bool texture) override;

	void move(Direction newDirection, const std::vector<Entity*> &walls, Apple* apple, float timeStep);
	// Actualiza la posición del gusano según la gravedad y verifica colisiones con el suelo
	void updateGravity(const std::vector<Entity*> &walls, float timeStep);
	// Verifica si el gusano está sobre algún bloque del mapa
	bool isOnGround(const std::vector<Entity*> &walls) const;
	// Reincia el gusano a una posición específica
	void reset(double x, double y, double z, int length);

	// Setters
	void setSpeed(double speed);
	void setGame(Game* game);  // Nuevo setter para game

	// Getters
	int getLength() const;
	double getSpeed() const;
	Direction getHeadDirection() const;
	// Retorna si el gusano está actualmente cayendo
	bool getIsFalling() const;
	// Retorna la velocidad vertical actual del gusano
	float getVerticalVelocity() const;
	// Retorna la posición Y desde donde comenzó la caída
	float getFallStartY() const;
	WormHead* getHead() const { return head; }
};




