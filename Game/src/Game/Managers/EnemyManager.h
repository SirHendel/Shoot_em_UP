#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Game/Entities/Enemy.h"
#include "BulletManager.h"
#include <vector>
#include <SDL3/SDL.h>

struct GameStats; // Forward declaration

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    
    // Constants moved from Game.cpp
    static const int ENEMIES_FOR_BOSS = 15;
    static const int BOSS_HEALTH = 10;
    static const Uint64 SHOOTER_DELAY = 3000;
    static const Uint64 BOSS_DELAY = 2500;

public:
    void spawnEnemy(EnemyType type, float difficultyMultiplier);
    void spawnBoss();
    void updateAll(float deltaTime, Uint64 currentTime, BulletManager& bulletManager);
    void renderAll(SDL_Renderer* renderer);
    void removeOffScreen(int& playerHealth);
    void clear();
    
    bool shouldSpawnBoss(const GameStats& stats) const;
    
    // For collision detection
    std::vector<Enemy>& getEnemies() { return enemies; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    
    size_t getEnemyCount() const { return enemies.size(); }
    bool isEmpty() const { return enemies.empty(); }
};

#endif