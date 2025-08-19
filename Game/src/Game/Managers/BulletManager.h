#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "Game/Entities/Bullet.h"
#include <vector>
#include <SDL3/SDL.h>

class BulletManager {
private:
    std::vector<Bullet> bullets;

public:
    void addBullet(float x, float y, bool isPlayerBullet = true);
    void updateAll(float deltaTime);
    void renderAll(SDL_Renderer* renderer);
    void removeOffScreen();
    void clear();
    
    // For collision detection - return reference to bullets vector
    std::vector<Bullet>& getBullets() { return bullets; }
    const std::vector<Bullet>& getBullets() const { return bullets; }
    
    size_t getBulletCount() const { return bullets.size(); }
};

#endif