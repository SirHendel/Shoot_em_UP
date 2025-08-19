#ifndef ENEMY_H
#define ENEMY_H

#include <SDL3/SDL.h>

enum EnemyType {
    BASIC_ENEMY,     // Normal enemy - 1 health, 10 points
    HEAVY_ENEMY,     // Tough enemy - 3 health, 25 points, slower
    FAST_ENEMY,      // Fast enemy - 1 health, 15 points, faster
    SHOOTER_ENEMY,   // Shoots at player - 2 health, 30 points
    BOSS_ENEMY       // Boss - high health, 100 points
};

class Enemy {
public:
    float x, y;
    int health;
    int maxHealth;
    float speed;
    int score;
    EnemyType type;
    Uint32 lastShotTime;
    
    Enemy(float startX, float startY, int hp, float spd, int scoreValue, EnemyType enemyType);
    
    void update(float deltaTime);
    void takeDamage(int damage);
    bool isAlive() const;
    bool isOffScreen() const;
    bool canShoot(Uint64 currentTime, Uint64 shootDelay) const;
    void updateLastShotTime(Uint64 currentTime);
    
    SDL_Color getColor() const;
    float getSize() const;
    
    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    EnemyType getType() const { return type; }
    int getScore() const { return score; }
};

#endif