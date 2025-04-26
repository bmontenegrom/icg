#include "Entity.h"



Entity::Entity(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
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
