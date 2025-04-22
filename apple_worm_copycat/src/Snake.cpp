#include "Snake.h"
#include <GL/gl.h>

const float Snake::SEGMENT_SIZE = 30.0f;

Snake::Snake(float startX, float startY) {
    float alignedX = static_cast<int>(startX / SEGMENT_SIZE) * SEGMENT_SIZE;
    float alignedY = static_cast<int>(startY / SEGMENT_SIZE) * SEGMENT_SIZE;
    body.push_back({alignedX, alignedY});
    speed = SEGMENT_SIZE;
    directionX = 1.0f;
    directionY = 0.0f;
}

void Snake::move() {
    // Move the head
    Position newHead = body[0];
    newHead.x += directionX * speed;
    newHead.y += directionY * speed;

    // Insert new head
    body.insert(body.begin(), newHead);
    
    // Remove tail
    body.pop_back();
}

void Snake::grow() {
    // Add a new segment at the tail position
    body.push_back(body.back());
}

void Snake::changeDirection(SDL_Keycode key) {
    switch (key) {
        case SDLK_UP:
            if (directionY != 1.0f) {
                directionX = 0.0f;
                directionY = -1.0f;
            }
            break;
        case SDLK_DOWN:
            if (directionY != -1.0f) {
                directionX = 0.0f;
                directionY = 1.0f;
            }
            break;
        case SDLK_LEFT:
            if (directionX != 1.0f) {
                directionX = -1.0f;
                directionY = 0.0f;
            }
            break;
        case SDLK_RIGHT:
            if (directionX != -1.0f) {
                directionX = 1.0f;
                directionY = 0.0f;
            }
            break;
    }
}

bool Snake::checkCollision(float x, float y) {
    for (const auto& segment : body) {
        if (segment.x == x && segment.y == y) {
            return true;
        }
    }
    return false;
}

const std::vector<Position>& Snake::getBody() const {
    return body;
}

Position Snake::getHead() const {
    return body[0];
}

void Snake::draw() const {
    // Draw border
    glColor3f(0.0f, 0.8f, 0.0f); // Darker green for border
    for (const auto& segment : body) {
        glBegin(GL_QUADS);
        glVertex2f(segment.x - 1, segment.y - 1);
        glVertex2f(segment.x + SEGMENT_SIZE + 1, segment.y - 1);
        glVertex2f(segment.x + SEGMENT_SIZE + 1, segment.y + SEGMENT_SIZE + 1);
        glVertex2f(segment.x - 1, segment.y + SEGMENT_SIZE + 1);
        glEnd();
    }

    // Draw main body
    glColor3f(0.0f, 1.0f, 0.0f); // Bright green
    for (const auto& segment : body) {
        glBegin(GL_QUADS);
        glVertex2f(segment.x, segment.y);
        glVertex2f(segment.x + SEGMENT_SIZE, segment.y);
        glVertex2f(segment.x + SEGMENT_SIZE, segment.y + SEGMENT_SIZE);
        glVertex2f(segment.x, segment.y + SEGMENT_SIZE);
        glEnd();
    }
} 
