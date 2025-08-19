#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "Game/Entities/Player.h"
#include "Game/Entities/Enemy.h"
#include "Game/Entities/Bullet.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include <vector>

struct GameStats; // Forward declaration

class CollisionManager {
public:
    // Check collision between two circular objects
    static bool checkCircleCollision(float x1, float y1, float r1, float x2, float y2, float r2);
    
    // Handle bullet-enemy collisions (player bullets hitting enemies)
    static void handleBulletEnemyCollisions(BulletManager& bulletManager, EnemyManager& enemyManager, 
                                          int& score, GameStats& stats);
    
    // Handle enemy bullets hitting player
    static void handleBulletPlayerCollisions(BulletManager& bulletManager, Player& player);
    
    // Handle direct enemy-player collisions
    static void handleEnemyPlayerCollisions(Player& player, EnemyManager& enemyManager);
};

#endif