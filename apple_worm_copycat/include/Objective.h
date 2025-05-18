#pragma once

#include "Entity.h"
#include "Particle.h"
#include <vector>

class Game; // Forward declaration

class Objective : public Entity
{
public:
	Objective(double x, double y, double z, double width, double height, double depth, Game* game = nullptr);
	~Objective();
	void render(bool texture);
	void renderParticles(bool texture);
	void setPaused(bool paused);
	void SetManzanaComida(bool comida);
	
	EntityType getType() const override { return EntityType::OBJECTIVE; }
private:
	std::vector<Particle*> particles;
	bool isPaused;
	bool manzanaComida;
	Game* game; // Referencia al juego
};