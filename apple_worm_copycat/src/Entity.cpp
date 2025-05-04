#include "Entity.h"
#include <iostream>
#include <cmath>


Entity::Entity(double x, double y, double z, double width, double height, double depth): x(x), y(y), z(z), width(width), height(height), depth(depth){
	
}

Entity::Entity(double x, double y, double z) : x(x), y(y), z(z), width(0.0f), height(0.0f), depth(0.0f)
{
}

Entity::Entity(double x, double y, double z, double width, double height, double depth, std::string path) : x(x), y(y), z(z), width(width), height(height), depth(depth)
{
	// Cargar el modelo desde el archivo
	ObjectLoader& loader = ObjectLoader::getInstance();
	this->vertices = loader.loadOBJ(path);
	if (this->vertices.empty()) {
		std::cerr << "Error al cargar el modelo desde el archivo: " << path << std::endl;
	}
}

Entity::~Entity()
{
}

void Entity::setPosition(double x, double y, double z){
	this->x = x;
	this->y = y;
	this->z = z;
}

void Entity::setX(double x){
	this->x = x;
}

void Entity::setY(double y){
	this->y = y;
}

void Entity::setZ(double z){
	this->z = z;
}

void Entity::setWidth(double width){
	this->width = width;
}

void Entity::setHeight(double height)
{
	this->height = height;
}

void Entity::setDepth(double depth)
{
	this->depth = depth;
}

double Entity::getX() const{
	return this->x;
}

double Entity::getY() const
{
	return this->y;
}

double Entity::getZ() const
{
	return this->z;
}

double Entity::getWidth() const
{
	return this->width;
}

double Entity::getHeight() const
{
	return this->height;
}

double Entity::getDepth() const
{
	return this->depth;
}

//todo poner cita
bool Entity::isColliding(const Entity& other) const
{
	bool collisionX = std::abs(this->x - other.x) < (this->width + other.width) / 2;
	bool collisionY = std::abs(this->y - other.y) < (this->height + other.height) / 2;
	bool collisionZ = std::abs(this->z - other.z) < (this->depth + other.depth) / 2;
	return collisionX && collisionY && collisionZ;
}
