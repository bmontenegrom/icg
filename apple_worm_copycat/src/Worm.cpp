#include "Worm.h"
#include "WormHead.h"
#include "WormTail.h"
#include "WormBody.h"

Worm::Worm(float x, float y, float z, Direction direction) : Entity(x, y, z), direction(direction) {
	this->head = new WormHead(x, y, z, direction);
	this->tail = new WormTail(x, y, z, direction);
	this->body = std::vector<Entity*>();
	this->body.push_back(head);
	for (int i = 0; i < 4; i++) {
		this->body.push_back(new WormBody(x, y, z, direction));
	}
	this->body.push_back(tail);
	this->size = this->body.size();
	this->speed = 1;

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

