#include "Entity.h"
#include <iostream>
#include <cmath>
#include "Constants.h"

float Entity::interpolation = 1.0f;

Entity::Entity(double x, double y, double z, double width, double height, double depth): x(x), y(y), z(z), width(width), height(height), depth(depth){
	this->prevX = x;
	this->prevY = y;
	this->prevZ = z;
}

Entity::Entity(double x, double y, double z) : x(x), y(y), z(z), width(0.0f), height(0.0f), depth(0.0f)
{
	this->prevX = x;
	this->prevY = y;
	this->prevZ = z;
}

Entity::Entity(double x, double y, double z, double width, double height, double depth, std::string path) : x(x), y(y), z(z), width(width), height(height), depth(depth)
{
	this->prevX = x;
	this->prevY = y;
	this->prevZ = z;
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

void Entity::setInterpolation(float interp) {
	interpolation = interp;
}

float Entity::getInterpolation() {
	return interpolation;
}

void Entity::updatePreviousPosition() {
	prevX = x;
	prevY = y;
	prevZ = z;
}

double Entity::getPrevX() const { return prevX; }
double Entity::getPrevY() const { return prevY; }
double Entity::getPrevZ() const { return prevZ; }

double Entity::getInterpolatedX() const {
	return prevX + (x - prevX) * interpolation;
}

double Entity::getInterpolatedY() const {
	return prevY + (y - prevY) * interpolation;
}

double Entity::getInterpolatedZ() const {
	return prevZ + (z - prevZ) * interpolation;
}

//todo poner cita
bool Entity::isColliding(const Entity* other) const
{
	if (!other) return false;
	
	
	const double WALL_MARGIN = 0.005f;     
	const double OBJECTIVE_MARGIN = 0.05f; 
	const double APPLE_MARGIN = 0.07f;     
	const double WORM_MARGIN = 0.09f;     
	
	
	double thisMargin = WALL_MARGIN;
	double otherMargin = WALL_MARGIN;
	
	
	if (this->getType() == EntityType::WORM) {
		thisMargin = WORM_MARGIN;
	} else if (this->getType() == EntityType::APPLE) {
		thisMargin = APPLE_MARGIN;
	} else if (this->getType() == EntityType::OBJECTIVE) {
		thisMargin = OBJECTIVE_MARGIN;
	}
	if (other->getType() == EntityType::WORM) {
		otherMargin = WORM_MARGIN;
	} else if (other->getType() == EntityType::APPLE) {
		otherMargin = APPLE_MARGIN;
	} else if (other->getType() == EntityType::OBJECTIVE) {
		otherMargin = OBJECTIVE_MARGIN;
	}
	
	
	double thisLeft = this->x - thisMargin/2;
	double thisRight = this->x + thisMargin/2;
	double thisTop = this->y + thisMargin/2;
	double thisBottom = this->y - thisMargin/2;
	
	double otherLeft = other->x - otherMargin/2;
	double otherRight = other->x + otherMargin/2;
	double otherTop = other->y + otherMargin/2;
	double otherBottom = other->y - otherMargin/2;
	
	
	const double COLLISION_MARGIN = 0.005f;  
	bool collisionX = (thisRight + COLLISION_MARGIN > otherLeft) && (thisLeft - COLLISION_MARGIN < otherRight);
	bool collisionY = (thisTop + COLLISION_MARGIN > otherBottom) && (thisBottom - COLLISION_MARGIN < otherTop);
	
	
	return collisionX && collisionY;
}
