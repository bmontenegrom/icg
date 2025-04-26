#include "Worm.h"
#include "WormHead.h"
#include "WormTail.h"
#include "WormBody.h"

Worm::Worm(float x, float y, float z) : Entity(x, y, z) {
	
	float size = 0.05f;
	Direction initialDirection = Direction::RIGHT; // Cambia la dirección inicial a la derecha
	this->head = new WormHead(x, y, z, size, initialDirection);
	this->body = std::vector<Entity*>();
	this->body.push_back(head);
	this->length = 5;

	for (int i = 1; i < length - 1; i++) {
		float bodyX = x - (i * size);
		float bodyY = y;
		float bodyZ = z;
		WormBody* segment = new WormBody(bodyX, bodyY, bodyZ, size, initialDirection);
		this->body.push_back(segment);
	}
	this->tail = new WormTail(x - (length - 1) * size, y, z, size, initialDirection);
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
	this->head->setDirection(newDirection);

	for (int i = body.size() - 1; i > 0; --i) {
		body[i]->setPosition(body[i - 1]->getX(), body[i - 1]->getY(), body[i - 1]->getZ());
	}

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
}



void Worm::setSpeed(float speed)
{
	this->speed = speed;
}



int Worm::getLength() const
{
	return this->length;
}

float Worm::getSpeed() const
{
	return this->speed;
}

Direction Worm::getHeadDirection() const
{
	return this->head->getDirection();
}

