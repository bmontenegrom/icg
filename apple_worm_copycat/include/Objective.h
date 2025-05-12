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
	void renderParticles(float speedMultiplier, bool texture);
	void setPaused(bool paused);
private:
	std::vector<Particle*> particles;
	bool isPaused;
	Game* game; // Referencia al juego
};