#pragma once

#include "Entity.h"


//https://lazyfoo.net/tutorials/SDL/38_particle_engines/index.php
class Particle : public Entity
{
public:
	Particle(double x, double y, double z);
	~Particle();
	void render() override;
	void update(float speedMultiplier);
	bool isDead() const;
private:
	float speedX;
	float speedY;
	float speedZ;
	unsigned int lifeTime;
	bool isAlive;
	unsigned int red;
	unsigned int green;
	unsigned int blue;

};