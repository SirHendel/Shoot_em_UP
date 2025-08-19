#include "Bullet.h"

const float Bullet::PLAYER_BULLET_SPEED = 8.0f;
const float Bullet::ENEMY_BULLET_SPEED = 4.0f;

Bullet::Bullet(float startX, float startY, bool isPlayer) 
    : x(startX), y(startY), isPlayerBullet(isPlayer) {}

void Bullet::update(float deltaTime) {
    if (isPlayerBullet) {
        y -= PLAYER_BULLET_SPEED; // Player bullets move up
    } else {
        y += ENEMY_BULLET_SPEED;  // Enemy bullets move down
    }
}

bool Bullet::isOffScreen() const {
    return y < -10 || y > 610;
}

SDL_Color Bullet::getColor() const {
    if (isPlayerBullet) {
        return {255, 255, 0, 255}; // Yellow for player bullets
    } else {
        return {255, 100, 100, 255}; // Light red for enemy bullets
    }
}