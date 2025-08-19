#include "Player.h"
#include <SDL3/SDL.h>

const float Player::MOVE_SPEED = 15.0f;
const float Player::SIZE = 15.0f;

Player::Player(float startX, float startY, int maxHP) 
    : x(startX), y(startY), health(maxHP), maxHealth(maxHP) {}

void Player::moveLeft() {
    if (x > SIZE) {
        x -= MOVE_SPEED;
    }
}

void Player::moveRight() {
    if (x < 800 - SIZE) { // Assuming 800 is screen width
        x += MOVE_SPEED;
    }
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Player::reset(float startX, float startY, int maxHP) {
    x = startX;
    y = startY;
    health = maxHP;
    maxHealth = maxHP;
}

bool Player::isAlive() const {
    return health > 0;
}

SDL_Color Player::getColor() const {
    return {0, 255, 0, 255}; // Green
}

float Player::getSize() const {
    return SIZE;
}