#pragma once
#include "Entity.h"
#include <vector>
#include "Constants.h"

class WormHead;
class WormBody;
class WormTail;
class Apple;
class Game;  

class Worm : public Entity {
private:
	WormHead* head;
	WormTail* tail;
	std::vector<Entity*> body;
	int length;
	double speed;
	bool isFalling;
	float verticalVelocity;
	float fallStartY;
	Game* game; 

public:
	Worm(double x, double y, double z, Game* game = nullptr);
	~Worm();
	void render(bool texture) override;
	EntityType getType() const override;

	void move(Direction newDirection, std::vector<Entity*>& entities, std::vector<Apple*>& apples, float timeStep);
	void updateGravity(const std::vector<Entity*> &walls, float timeStep);
	bool isOnGround(const std::vector<Entity*> &walls) const;
	void reset(double x, double y, double z, int length);


	void setSpeed(double speed);
	void setGame(Game* game);  

	
	int getLength() const;
	double getSpeed() const;
	Direction getHeadDirection() const;
	bool getIsFalling() const;
	float getVerticalVelocity() const;
	float getFallStartY() const;
	WormHead* getHead() const { return head; }
};




