#pragma once

#include "Entity.h"
#include "Particle.h"
#include <vector>

class Objective : public Entity
{
public:
	Objective(double x, double y, double z, double width, double height, double depth);
	~Objective();
	void render();
	void renderParticles();
	void setPaused(bool paused);
private:
	std::vector<Particle*> particles;
	bool isPaused;
};