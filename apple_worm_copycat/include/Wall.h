#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <vector>
#include "ObjLoader.h"
#include "Entity.h"

class Wall : public Entity {
public:
	Wall(double x, double y, double z, double width, double height, double depth);
	void render(bool texture) override;
	EntityType getType() const override { return EntityType::WALL; }
private:
	std::vector<ObjVertex> vertices;
	GLuint texturaMuro;
	GLuint cargarTextura(const char* path);
};