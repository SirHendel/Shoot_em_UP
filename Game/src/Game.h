#ifndef GAME_H
#define GAME_H

#include "AppContext.h"
#include "GameState.h"
#include "Translations.h"
#include <vector>
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
    
    Enemy(float x, float y, int health, float speed, int score, EnemyType type)
        : x(x), y(y), health(health), maxHealth(health), speed(speed), score(score), type(type), lastShotTime(0) {}
    
    SDL_Color getColor() const;
    float getSize() const;
};

struct Player {
    float x, y;
    int health;
    int maxHealth;
};

struct Bullet {
    float x, y;
    bool isPlayerBullet; // true for player bullets, false for enemy bullets
    
    Bullet(float x, float y, bool isPlayer = true) : x(x), y(y), isPlayerBullet(isPlayer) {}
};

// Game statistics
struct GameStats {
    int enemiesKilled;
    int bossesDefeated;
    int totalEnemiesSpawned;
    bool bossActive;
    Uint32 gameStartTime;
};

extern int score;
extern GameStats gameStats;

// Game difficulty scaling
extern int currentWave;
extern float difficultyMultiplier;

void initializeGame();
void resetGame();
void renderGame(AppContext& appContext);
void handleGameEvents(AppContext& appContext, SDL_Event* event);
void updateGameState(AppContext& appContext);
void shootBullet(std::vector<Bullet>& bullets, float playerX, float playerY, bool isPlayerBullet = true);
void spawnEnemy(EnemyType type = BASIC_ENEMY);
void spawnBoss();
void checkGameOverConditions(AppContext& appContext);
void updateDifficulty();

#endif