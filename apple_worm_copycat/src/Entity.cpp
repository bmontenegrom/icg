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
	// Calcular los límites de cada entidad con un margen más grande para evitar colisiones falsas
	const double MARGIN = 0.01f;  // Margen aún más grande para eliminar colisiones por contacto mínimo
	
	// Calcular los límites considerando el centro de cada entidad
	double thisLeft = this->x - this->width/2 + MARGIN;
	double thisRight = this->x + this->width/2 - MARGIN;
	double thisTop = this->y + this->height/2 - MARGIN;
	double thisBottom = this->y - this->height/2 + MARGIN;
	
	double otherLeft = other.x - other.width/2 + MARGIN;
	double otherRight = other.x + other.width/2 - MARGIN;
	double otherTop = other.y + other.height/2 - MARGIN;
	double otherBottom = other.y - other.height/2 + MARGIN;
	
	// Verificar si hay superposición en X e Y usando > y < para evitar colisiones por contacto mínimo
	bool collisionX = (thisRight > otherLeft) && (thisLeft < otherRight);
	bool collisionY = (thisTop > otherBottom) && (thisBottom < otherTop);
	
	// Debug: imprimir información si hay colisión
	if (collisionX && collisionY) {
		std::cout << "Colisión entre: "
				  << "Entidad1 (" << this->x << ", " << this->y << ", " << this->z << ", " << this->width << ", " << this->height << ", " << this->depth << ") y "
				  << "Entidad2 (" << other.x << ", " << other.y << ", " << other.z << ", " << other.width << ", " << other.height << ", " << other.depth << ")" << std::endl;
	}
	
	// Solo hay colisión si hay superposición en ambos ejes
	return collisionX && collisionY;
}
