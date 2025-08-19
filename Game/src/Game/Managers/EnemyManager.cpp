#include "EnemyManager.h"
#include "Game/Game.h" 
#include <cstdlib>
#include <algorithm>

const int EnemyManager::ENEMIES_FOR_BOSS;
const int EnemyManager::BOSS_HEALTH;
const Uint64 EnemyManager::SHOOTER_DELAY;
const Uint64 EnemyManager::BOSS_DELAY;

void EnemyManager::spawnEnemy(EnemyType type, float difficultyMultiplier) {
    float x = static_cast<float>(rand() % 750 + 25); // Keep enemies on screen
    float speed;
    int health, scoreValue;
    
    switch (type) {
        case BASIC_ENEMY:
            speed = 0.8f * difficultyMultiplier;
            health = 1;
            scoreValue = 10;
            break;
        case HEAVY_ENEMY:
            speed = 0.3f * difficultyMultiplier;
            health = 2;
            scoreValue = 25;
            break;
        case FAST_ENEMY:
            speed = 1.8f * difficultyMultiplier;
            health = 1;
            scoreValue = 15;
            break;
        case SHOOTER_ENEMY:
            speed = 0.5f * difficultyMultiplier;
            health = 2;
            scoreValue = 30;
            break;
        default:
            speed = 0.8f;
            health = 1;
            scoreValue = 10;
            break;
    }
    
    enemies.emplace_back(x, -30, health, speed, scoreValue, type);
}

void EnemyManager::spawnBoss() {
    float x = 400; // Center of screen
    enemies.emplace_back(x, -50, BOSS_HEALTH, 0.5f, 100, BOSS_ENEMY);
}

void EnemyManager::updateAll(float deltaTime, Uint64 currentTime, BulletManager& bulletManager) {
    for (auto& enemy : enemies) {
        enemy.update(deltaTime);
        
        // Handle shooting behavior
        if (enemy.type == SHOOTER_ENEMY && enemy.canShoot(currentTime, SHOOTER_DELAY)) {
            if (enemy.y > 50 && enemy.y < 400) { // Only shoot when in good range
                bulletManager.addBullet(enemy.x, enemy.y + 20, false);
                enemy.updateLastShotTime(currentTime);
            }
        }
        
        // Boss shooting pattern
        if (enemy.type == BOSS_ENEMY && enemy.canShoot(currentTime, BOSS_DELAY)) {
            // Shoot fewer bullets
            for (int i = -1; i <= 1; i++) {
                bulletManager.addBullet(enemy.x + i * 25, enemy.y + 40, false);
            }
            enemy.updateLastShotTime(currentTime);
        }
    }
}

void EnemyManager::renderAll(SDL_Renderer* renderer) {
    // This will be moved to GameRenderer later, but for now keep rendering logic here
    for (const auto& enemy : enemies) {
        SDL_Color enemyColor = enemy.getColor();
        float size = enemy.getSize();
        
        // You'll need to call the renderTriangle function from Game.cpp
        // For now, we'll use a simple rectangle
        SDL_SetRenderDrawColor(renderer, enemyColor.r, enemyColor.g, enemyColor.b, enemyColor.a);
        SDL_FRect enemyRect = {enemy.x - size/2, enemy.y - size/2, size, size};
        SDL_RenderFillRect(renderer, &enemyRect);
        
        // Health bar rendering will also be moved to GameRenderer
        if (enemy.maxHealth > 1) {
            // Background (red)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_FRect bgRect = {enemy.x - size, enemy.y - 25, size * 2, 4};
            SDL_RenderFillRect(renderer, &bgRect);
            
            // Health (green)
            if (enemy.health > 0) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                float healthWidth = (static_cast<float>(enemy.health) / enemy.maxHealth) * size * 2;
                SDL_FRect healthRect = {enemy.x - size, enemy.y - 25, healthWidth, 4};
                SDL_RenderFillRect(renderer, &healthRect);
            }
        }
    }
}

void EnemyManager::removeOffScreen(int& playerHealth) {
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->isOffScreen()) {
            playerHealth -= (it->type == BOSS_ENEMY) ? 2 : 1;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

void EnemyManager::clear() {
    enemies.clear();
}

bool EnemyManager::shouldSpawnBoss(const GameStats& stats) const {
    return !stats.bossActive && stats.enemiesKilled > 0 && 
           stats.enemiesKilled % ENEMIES_FOR_BOSS == 0;
}