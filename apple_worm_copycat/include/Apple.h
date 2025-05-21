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
	void render(bool texture) override;
	EntityType getType() const override;
	void reset(double x, double y, double z);
	bool eaten() const;
	void setEaten(bool eaten);

private:
	bool isEaten;
	std::vector<ObjVertex> vertices;
	GLuint texturaManzana;
	GLuint cargarTextura(const char* path);
};