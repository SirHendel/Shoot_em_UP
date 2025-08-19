#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>

class Player {
private:
    static const float MOVE_SPEED;
    static const float SIZE;

public:
    float x, y;
    int health;
    int maxHealth;

    Player(float startX, float startY, int maxHP);
    
    void moveLeft();
    void moveRight();
    void takeDamage(int damage);
    void reset(float startX, float startY, int maxHP);
    
    bool isAlive() const;
    SDL_Color getColor() const;
    float getSize() const;
    
    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
};

#endif