#include "CollisionManager.h"
#include "Game/Game.h"
#include "Game/Entities/Player.h"
#include <cmath>
#include <algorithm>

bool CollisionManager::checkCircleCollision(float x1, float y1, float r1, float x2, float y2, float r2) {
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < (r1 + r2);
}

void CollisionManager::handleBulletEnemyCollisions(BulletManager& bulletManager, EnemyManager& enemyManager, 
                                                  int& score, GameStats& stats) {
    auto& bullets = bulletManager.getBullets();
    auto& enemies = enemyManager.getEnemies();
    
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool bulletRemoved = false;
        
        if (bulletIt->isPlayerBullet) {
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
                float hitRadius = enemyIt->getSize();
                
                if (checkCircleCollision(bulletIt->x, bulletIt->y, 2.0f, 
                                       enemyIt->x, enemyIt->y, hitRadius)) {
                    enemyIt->takeDamage(1);
                    bulletIt = bullets.erase(bulletIt);
                    bulletRemoved = true;
                    
                    if (!enemyIt->isAlive()) {
                        score += enemyIt->getScore();
                        stats.enemiesKilled++;
                        
                        if (enemyIt->getType() == BOSS_ENEMY) {
                            stats.bossesDefeated++;
                            stats.bossActive = false;
                        }
                        
                        enemyIt = enemies.erase(enemyIt);
                    } else {
                        ++enemyIt;
                    }
                    break;
                } else {
                    ++enemyIt;
                }
            }
        }
        
        if (!bulletRemoved) {
            ++bulletIt;
        }
    }
}

void CollisionManager::handleBulletPlayerCollisions(BulletManager& bulletManager, Player& player) {
    auto& bullets = bulletManager.getBullets();
    
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        if (!bulletIt->isPlayerBullet) {
            if (checkCircleCollision(bulletIt->x, bulletIt->y, 2.0f, 
                                   player.x, player.y, player.getSize())) {
                player.takeDamage(1);
                bulletIt = bullets.erase(bulletIt);
                continue;
            }
        }
        ++bulletIt;
    }
}

void CollisionManager::handleEnemyPlayerCollisions(Player& player, EnemyManager& enemyManager) {
    auto& enemies = enemyManager.getEnemies();
    
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        if (checkCircleCollision(player.x, player.y, player.getSize(),
                               enemyIt->x, enemyIt->y, enemyIt->getSize())) {
            int damage = (enemyIt->getType() == BOSS_ENEMY) ? 3 : 1;
            player.takeDamage(damage);
            enemyIt = enemies.erase(enemyIt);
        } else {
            ++enemyIt;
        }
    }
}