#pragma once
#include "Entity.h"
#include <vector>
#include "Constants.h"

class WormHead;
class WormBody;
class WormTail;
class Apple;

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

public:
	Worm(double x, double y, double z);
	~Worm();
	void render() override;

	void move(Direction newDirection, const std::vector<Entity*> &walls, Apple* apple, float timeStep);
	// Actualiza la posición del gusano según la gravedad y verifica colisiones con el suelo
	void updateGravity(const std::vector<Entity*> &walls, float timeStep);
	// Verifica si el gusano está sobre algún bloque del mapa
	bool isOnGround(const std::vector<Entity*> &walls) const;
	// Reinicia el gusano a su posición inicial cuando cae al vacío
	void reset();

	// Setters
	void setSpeed(double speed);
	

	// Getters
	int getLength() const;
	double getSpeed() const;
	Direction getHeadDirection() const;
	// Retorna si el gusano está actualmente cayendo
	bool getIsFalling() const;


};




