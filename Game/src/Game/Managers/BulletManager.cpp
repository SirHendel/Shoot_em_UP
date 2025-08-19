#include "BulletManager.h"
#include <algorithm>

void BulletManager::addBullet(float x, float y, bool isPlayerBullet) {
    bullets.emplace_back(x, y, isPlayerBullet);
}

void BulletManager::updateAll(float deltaTime) {
    for (auto& bullet : bullets) {
        bullet.update(deltaTime);
    }
}

void BulletManager::renderAll(SDL_Renderer* renderer) {
    for (const auto& bullet : bullets) {
        SDL_Color color = bullet.getColor();
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        
        const SDL_FRect bulletRect = {bullet.x - 2, bullet.y - 5, 4, 10};
        SDL_RenderFillRect(renderer, &bulletRect);
    }
}

void BulletManager::removeOffScreen() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [](const Bullet& bullet) {
            return bullet.isOffScreen();
        }), bullets.end());
}

void BulletManager::clear() {
    bullets.clear();
}