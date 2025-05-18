#pragma once
#include <windows.h>
#include <GL/gl.h>
#include "Entity.h"


//https://lazyfoo.net/tutorials/SDL/38_particle_engines/index.php
class Particle : public Entity
{
public:
	Particle(double x, double y, double z);
	~Particle();
	void render(bool texture) override;
	void update();
	bool isDead() const;
private:
	float speedX;
	float speedY;
	float speedZ;
	unsigned int lifeTime;
	bool isAlive;
	GLuint texturaParticulaRed;
	GLuint texturaParticulaGreen;
	GLuint texturaParticulaBlue;
	GLuint cargarTextura(const char* path);

};