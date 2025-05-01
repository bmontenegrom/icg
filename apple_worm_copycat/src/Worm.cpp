#include "Worm.h"
#include "WormHead.h"
#include "WormTail.h"
#include "WormBody.h"
#include "Apple.h"
#include <iostream>

Worm::Worm(double x, double y, double z) : Entity(x, y, z) {
	
	double segment_size = 0.05f;
	Direction initialDirection = Direction::RIGHT; // dirección inicial a la derecha
	this->head = new WormHead(x, y, z, segment_size, segment_size, segment_size, initialDirection);
	this->body = std::vector<Entity*>();
	this->body.push_back(head);
	this->length = 5;

	for (int i = 1; i < length - 1; i++) {
		double bodyX = x - (i * segment_size );
		double bodyY = y;
		double bodyZ = z;
		WormBody* segment = new WormBody(bodyX, bodyY, bodyZ, segment_size, segment_size, segment_size, initialDirection);
		this->body.push_back(segment);
	}
	this->tail = new WormTail(x - (length - 1) * segment_size , y, z, segment_size,segment_size , segment_size, initialDirection);
	this->body.push_back(tail);
	this->speed = 3.2f;
}

Worm::~Worm() {
	for (Entity* segment : body) {
		delete segment;
	}
}

void Worm::render(){
	for (Entity* segment : body) {
		segment->render();
	}
}

void Worm::move(Direction newDirection, const std::vector<Entity*> &walls, Apple* apple, float timeStep)
{
	if (this->head->getDirection() == Direction::UP && newDirection == Direction::DOWN ||
		this->head->getDirection() == Direction::DOWN && newDirection == Direction::UP ||
		this->head->getDirection() == Direction::LEFT && newDirection == Direction::RIGHT ||
		this->head->getDirection() == Direction::RIGHT && newDirection == Direction::LEFT) {
		return; // No se puede mover en la dirección opuesta
	}
	double oldX = this->head->getX();
	double oldY = this->head->getY();
	double oldZ = this->head->getZ();
	Direction oldDirection = this->head->getDirection();

	this->head->setDirection(newDirection);

	float distance = this->speed * timeStep;
	


	switch (this->getHeadDirection())
	{
	case UP:
		this->head->setY(this->head->getY() + distance);
		break;
	case DOWN:
		this->head->setY(this->head->getY() - distance);
		break;
	case LEFT:
		this->head->setX(this->head->getX() - distance);
		break;
	case RIGHT:
		this->head->setX(this->head->getX() + distance);
		break;
	}

	// Checkea si el gusano choca con su propio cuerpo
	for (int i = 1; i < body.size(); ++i) {
		if (this->head->isColliding(*body[i])) {
			this->head->setPosition(oldX, oldY, oldZ);
			this->head->setDirection(oldDirection);
			return;
		}
	}

	// Checkea si el gusano choca con las paredes
	for (auto wall : walls) {
		if (this->head->isColliding(*wall)) {
			this->head->setPosition(oldX, oldY, oldZ);
			this->head->setDirection(oldDirection);
			return;
		}
	}

	// Checkea si el gusano choca con la manzana
	if (apple != nullptr && this->head->isColliding(*apple)) {
		//apple->setPosition(rand() % 20 * 0.05, rand() % 20 * 0.05, 0.0);
		this->length++;
		double bodyX = this->body[this->length - 2]->getX();
		double bodyY = this->body[this->length - 2]->getY();
		double bodyZ = this->body[this->length - 2]->getZ();
		WormBody* segment = new WormBody(bodyX, bodyY, bodyZ, this->head->getWidth(), this->head->getHeight(), this->head->getDepth(), this->head->getDirection());
		this->body.insert(this->body.end() - 1, segment);
		apple->setEaten(true);
	}

	for (int i = body.size() - 1; i > 0; --i) {
		body[i]->setPosition(body[i - 1]->getX(), body[i - 1]->getY(), body[i - 1]->getZ());
	}
	this->setPosition(this->head->getX(), this->head->getY(), this->head->getZ());
}



void Worm::setSpeed(double speed)
{
	this->speed = speed;
}



int Worm::getLength() const
{
	return this->length;
}

double Worm::getSpeed() const
{
	return this->speed;
}

Direction Worm::getHeadDirection() const
{
	return this->head->getDirection();
}

