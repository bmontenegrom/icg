#include "Entity.h"
#include <cmath>


Entity::Entity(float x, float y, float z, float width, float height, float depth): x(x), y(y), z(z), width(width), height(height), depth(depth){
	
}

Entity::Entity(float x, float y, float z) : x(x), y(y), z(z), width(0.0f), height(0.0f), depth(0.0f)
{
}

Entity::~Entity()
{
}

void Entity::setPosition(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}

void Entity::setX(float x){
	this->x = x;
}

void Entity::setY(float y){
	this->y = y;
}

void Entity::setZ(float z){
	this->z = z;
}

void Entity::setWidth(float width){
	this->width = width;
}

void Entity::setHeight(float height)
{
	this->height = height;
}

void Entity::setDepth(float depth)
{
	this->depth = depth;
}

float Entity::getX() const{
	return this->x;
}

float Entity::getY() const
{
	return this->y;
}

float Entity::getZ() const
{
	return this->z;
}

float Entity::getWidth() const
{
	return this->width;
}

float Entity::getHeight() const
{
	return this->height;
}

float Entity::getDepth() const
{
	return this->depth;
}

bool Entity::isColliding(const Entity& other) const
{
	bool collisionX = std::abs(this->x - other.x) < (this->width + other.width) / 2;
	bool collisionY = std::abs(this->y - other.y) < (this->height + other.height) / 2;
	bool collisionZ = std::abs(this->z - other.z) < (this->depth + other.depth) / 2;
	return collisionX && collisionY && collisionZ;
}
