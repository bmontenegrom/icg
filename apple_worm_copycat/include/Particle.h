#pragma once

#include "Entity.h"

class Particle : public Entity
{
public:
	Particle(double x, double y, double z);
	~Particle();
	void render() override;
	void update();
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