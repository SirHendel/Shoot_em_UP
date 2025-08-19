#ifndef BULLET_H
#define BULLET_H

#include <SDL3/SDL.h>

class Bullet {
private:
    static const float PLAYER_BULLET_SPEED;
    static const float ENEMY_BULLET_SPEED;
    
public:
    float x, y;
    bool isPlayerBullet;
    
    Bullet(float startX, float startY, bool isPlayer = true);
    
    void update(float deltaTime);
    bool isOffScreen() const;
    SDL_Color getColor() const;
    
    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    bool getIsPlayerBullet() const { return isPlayerBullet; }
};

#endif