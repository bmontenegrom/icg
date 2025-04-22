#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <SDL.h>

struct Position {
    float x, y;
};

class Snake {
public:
    Snake(float startX, float startY);
    void move();
    void grow();
    void changeDirection(SDL_Keycode key);
    bool checkCollision(float x, float y);
    const std::vector<Position>& getBody() const;
    Position getHead() const;
    void draw() const;

private:
    std::vector<Position> body;
    float speed;
    float directionX;
    float directionY;
    static const float SEGMENT_SIZE;
};

#endif // SNAKE_H 
