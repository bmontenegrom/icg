#include "Worm.h"
#include "WormHead.h"
#include "WormTail.h"
#include "WormBody.h"
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
	this->speed = 0.05f;
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

void Worm::move(Direction newDirection)
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
	


	switch (this->getHeadDirection())
	{
	case UP:
		this->head->setY(this->head->getY() + this->speed);
		break;
	case DOWN:
		this->head->setY(this->head->getY() - this->speed);
		break;
	case LEFT:
		this->head->setX(this->head->getX() - this->speed);
		break;
	case RIGHT:
		this->head->setX(this->head->getX() + this->speed);
		break;
	}

	// Check if the worm is colliding with itself
	for (int i = 1; i < body.size(); ++i) {
		if (this->head->isColliding(*body[i])) {
			
			this->head->setPosition(oldX, oldY, oldZ);
			this->head->setDirection(oldDirection);
			return;
		}
	}


	for (int i = body.size() - 1; i > 0; --i) {
		body[i]->setPosition(body[i - 1]->getX(), body[i - 1]->getY(), body[i - 1]->getZ());
	}
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

