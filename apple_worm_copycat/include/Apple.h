#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "ObjLoader.h"
#include "Entity.h"

class Apple : public Entity {
public:
	Apple(double x, double y, double z, double width, double height, double depth);
	~Apple();
	void render() override;
	bool eaten();
	void setEaten(bool eaten);

private:
	bool isEaten;
	std::vector<ObjVertex> vertices;
	GLuint texturaManzana;
	GLuint cargarTextura(const char* path);
};