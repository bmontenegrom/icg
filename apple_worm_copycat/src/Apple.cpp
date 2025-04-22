#include "Apple.h"
#include <GL/gl.h>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float Apple::SIZE = 30.0f;

Apple::Apple() {
    std::srand(std::time(nullptr));
    position = {0.0f, 0.0f};
}

void Apple::respawn(const Snake& snake) {
    bool validPosition = false;
    while (!validPosition) {
        position.x = (std::rand() % (WINDOW_WIDTH / static_cast<int>(SIZE))) * SIZE;
        position.y = (std::rand() % (WINDOW_HEIGHT / static_cast<int>(SIZE))) * SIZE;
        
        // Need to get non-const reference to call non-const member function
        Snake& nonConstSnake = const_cast<Snake&>(snake);
        validPosition = !nonConstSnake.checkCollision(position.x, position.y);
    }
}

void Apple::draw() const {
    // Draw border
    glColor3f(0.8f, 0.0f, 0.0f); // Darker red for border
    glBegin(GL_QUADS);
    glVertex2f(position.x - 1, position.y - 1);
    glVertex2f(position.x + SIZE + 1, position.y - 1);
    glVertex2f(position.x + SIZE + 1, position.y + SIZE + 1);
    glVertex2f(position.x - 1, position.y + SIZE + 1);
    glEnd();

    // Draw main apple
    glColor3f(1.0f, 0.0f, 0.0f); // Bright red
    glBegin(GL_QUADS);
    glVertex2f(position.x, position.y);
    glVertex2f(position.x + SIZE, position.y);
    glVertex2f(position.x + SIZE, position.y + SIZE);
    glVertex2f(position.x, position.y + SIZE);
    glEnd();
}

Position Apple::getPosition() const {
    return position;
}
