#ifndef APPLE_H
#define APPLE_H

#include "Snake.h"

class Apple {
public:
    Apple();
    void respawn(const Snake& snake);
    void draw() const;
    Position getPosition() const;

private:
    Position position;
    static const float SIZE;
};

#endif // APPLE_H 
