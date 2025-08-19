#include "Enemy.h"
#include <SDL3/SDL.h>

Enemy::Enemy(float startX, float startY, int hp, float spd, int scoreValue, EnemyType enemyType)
    : x(startX), y(startY), health(hp), maxHealth(hp), speed(spd), score(scoreValue), 
      type(enemyType), lastShotTime(0) {}

void Enemy::update(float deltaTime) {
    y += speed;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

bool Enemy::isAlive() const {
    return health > 0;
}

bool Enemy::isOffScreen() const {
    return y > 600; // Assuming 600 is screen height
}

bool Enemy::canShoot(Uint64 currentTime, Uint64 shootDelay) const {
    return (type == SHOOTER_ENEMY || type == BOSS_ENEMY) && 
           (currentTime - lastShotTime > shootDelay);
}

void Enemy::updateLastShotTime(Uint64 currentTime) {
    lastShotTime = currentTime;
}

SDL_Color Enemy::getColor() const {
    switch (type) {
        case BASIC_ENEMY:
            return {255, 0, 0, 255}; // Red
        case HEAVY_ENEMY:
            return {255, 165, 0, 255}; // Orange
        case FAST_ENEMY:
            return {255, 255, 0, 255}; // Yellow
        case SHOOTER_ENEMY:
            return {255, 0, 255, 255}; // Magenta
        case BOSS_ENEMY:
            return {128, 0, 128, 255}; // Purple
        default:
            return {255, 0, 0, 255}; // Default red
    }
}

float Enemy::getSize() const {
    switch (type) {
        case BASIC_ENEMY:
            return 15.0f;
        case HEAVY_ENEMY:
            return 20.0f;
        case FAST_ENEMY:
            return 12.0f;
        case SHOOTER_ENEMY:
            return 18.0f;
        case BOSS_ENEMY:
            return 40.0f;
        default:
            return 15.0f;
    }
}