#include "Game/Game.h"
#include "Game/Entities/Player.h"
#include "Game/Managers/BulletManager.h"
#include "Game/Managers/EnemyManager.h"
#include "Game/Managers/CollisionManager.h"
#include "Core/Utils.h"
#include <cstdlib>
#include <ctime>
#include <SDL3/SDL.h>
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <cmath>

// Game objects
Player player(400, 500, 15);
BulletManager bulletManager;
EnemyManager enemyManager;

// Game state variables
int score = 0;
GameStats gameStats = {0, 0, 0, false, 0};
int currentWave = 1;
float difficultyMultiplier = 1.0f;

// Timing variables
Uint64 lastEnemySpawnTime = 0;
const Uint64 basicEnemySpawnDelay = 3000;
Uint64 lastWaveCheckTime = 0;
const Uint64 waveCheckDelay = 15000;

// Rendering functions (will be moved to GameRenderer later)
void renderTriangle(SDL_Renderer* renderer, float x, float y, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_Vertex vert[3];
    vert[0] = { {x, y - size}, {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b), static_cast<float>(color.a)} };
    vert[1] = { {x - size, y + size}, {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b), static_cast<float>(color.a)} };
    vert[2] = { {x + size, y + size}, {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b), static_cast<float>(color.a)} };

    SDL_RenderGeometry(renderer, nullptr, vert, 3, nullptr, 0);
}

void initializeGame() {
    srand(static_cast<unsigned int>(time(nullptr)));
    resetGame();
}

void resetGame() {
    player.reset(400, 500, 15);
    bulletManager.clear();
    enemyManager.clear();
    score = 0;
    gameStats = {0, 0, 0, false, SDL_GetTicks()};
    currentWave = 1;
    difficultyMultiplier = 1.0f;
    lastEnemySpawnTime = 0;
    lastWaveCheckTime = SDL_GetTicks();
}

void renderGame(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);

    // Render player
    SDL_Color playerColor = player.getColor();
    renderTriangle(appContext.renderer, player.x, player.y, player.getSize(), playerColor);

    // Render enemies (using temporary rendering until GameRenderer is implemented)
    for (const auto& enemy : enemyManager.getEnemies()) {
        SDL_Color enemyColor = enemy.getColor();
        float size = enemy.getSize();
        renderTriangle(appContext.renderer, enemy.x, enemy.y, size, enemyColor);
        
        // Render health bar for enemies with more than 1 health
        if (enemy.maxHealth > 1) {
            // Background (red)
            SDL_SetRenderDrawColor(appContext.renderer, 255, 0, 0, 255);
            SDL_FRect bgRect = {enemy.x - size, enemy.y - 25, size * 2, 4};
            SDL_RenderFillRect(appContext.renderer, &bgRect);
            
            // Health (green)
            if (enemy.health > 0) {
                SDL_SetRenderDrawColor(appContext.renderer, 0, 255, 0, 255);
                float healthWidth = (static_cast<float>(enemy.health) / enemy.maxHealth) * size * 2;
                SDL_FRect healthRect = {enemy.x - size, enemy.y - 25, healthWidth, 4};
                SDL_RenderFillRect(appContext.renderer, &healthRect);
            }
        }
    }

    // Render bullets
    bulletManager.renderAll(appContext.renderer);

    // Render UI with translations
    SDL_Color textColor = {0, 255, 0, 255};
    
    std::string scoreText = getTranslation(GAME_SCORE, selectedLanguage) + ": " + std::to_string(score);
    renderGameText(appContext.renderer, scoreText.c_str(), 10, 10, appContext.font, textColor);

    std::string healthText = getTranslation(GAME_HEALTH, selectedLanguage) + ": " + std::to_string(player.getHealth());
    renderGameText(appContext.renderer, healthText.c_str(), 10, 40, appContext.font, textColor);

    std::string waveText = getTranslation(GAME_WAVE, selectedLanguage) + ": " + std::to_string(currentWave);
    renderGameText(appContext.renderer, waveText.c_str(), 10, 70, appContext.font, textColor);

    std::string killsText = getTranslation(GAME_KILLS, selectedLanguage) + ": " + std::to_string(gameStats.enemiesKilled);
    renderGameText(appContext.renderer, killsText.c_str(), 10, 100, appContext.font, textColor);

    if (gameStats.bossActive) {
        SDL_Color bossWarning = {255, 255, 0, 255};
        std::string bossText = getTranslation(GAME_BOSS_FIGHT, selectedLanguage);
        renderGameText(appContext.renderer, bossText.c_str(), 300, 50, appContext.font, bossWarning);
    }

    // Show escape instruction
    SDL_Color instructionColor = {200, 200, 200, 255};
    std::string escText = getTranslation(GAME_PRESS_ESC, selectedLanguage);
    renderGameText(appContext.renderer, escText.c_str(), 10, 550, appContext.font, instructionColor);

    SDL_RenderPresent(appContext.renderer);
}

void handleGameEvents(AppContext& appContext, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_LEFT:
                player.moveLeft();
                break;
            case SDL_SCANCODE_RIGHT:
                player.moveRight();
                break;
            case SDL_SCANCODE_SPACE:
                bulletManager.addBullet(player.getX(), player.getY(), true);
                break;
            case SDL_SCANCODE_ESCAPE:
                appContext.currentState = MAIN_MENU;
                break;
        }
    }
}

void updateDifficulty() {
    Uint64 currentTime = SDL_GetTicks();
    if (currentTime - lastWaveCheckTime > waveCheckDelay) {
        currentWave++;
        difficultyMultiplier += 0.1f;
        lastWaveCheckTime = currentTime;
    }
}

void checkGameOverConditions(AppContext& appContext) {
    if (!player.isAlive()) {
        appContext.currentState = GAME_OVER;
        return;
    }
    
    // Win condition: defeat 2 bosses
    if (gameStats.bossesDefeated >= 2) {
        appContext.currentState = GAME_WON;
        return;
    }
}

EnemyType chooseEnemyType(int currentWave) {
    EnemyType typeToSpawn = BASIC_ENEMY;
    int randChoice = rand() % 100;
    
    if (currentWave >= 2) {
        if (randChoice < 15) typeToSpawn = HEAVY_ENEMY;
        else if (randChoice < 30) typeToSpawn = FAST_ENEMY;
        else if (randChoice < 40 && currentWave >= 3) typeToSpawn = SHOOTER_ENEMY;
    }
    
    return typeToSpawn;
}

void updateGameState(AppContext& appContext) {
    Uint64 currentTime = SDL_GetTicks();
    
    // Update difficulty over time
    updateDifficulty();
    
    // Check for boss spawn
    if (enemyManager.shouldSpawnBoss(gameStats)) {
        enemyManager.clear(); // Clear existing enemies before spawning boss
        enemyManager.spawnBoss();
        gameStats.bossActive = true;
    }
    
    // Spawn regular enemies if no boss is active
    if (!gameStats.bossActive && currentTime - lastEnemySpawnTime > basicEnemySpawnDelay / difficultyMultiplier) {
        EnemyType typeToSpawn = chooseEnemyType(currentWave);
        enemyManager.spawnEnemy(typeToSpawn, difficultyMultiplier);
        gameStats.totalEnemiesSpawned++;
        lastEnemySpawnTime = currentTime;
    }

    // Update all game objects
    float deltaTime = 1.0f; // You might want to calculate actual delta time
    enemyManager.updateAll(deltaTime, currentTime, bulletManager);
    bulletManager.updateAll(deltaTime);
    
    // Handle all collisions
    CollisionManager::handleBulletEnemyCollisions(bulletManager, enemyManager, score, gameStats);
    CollisionManager::handleBulletPlayerCollisions(bulletManager, player);
    CollisionManager::handleEnemyPlayerCollisions(player, enemyManager);

    // Remove off-screen objects
    bulletManager.removeOffScreen();
    enemyManager.removeOffScreen(player.health); // This will damage player if enemies pass

    // Check game over conditions
    checkGameOverConditions(appContext);
}

// Legacy functions for compatibility (these call the new systems)
void shootBullet(std::vector<Bullet>& bullets, float playerX, float playerY, bool isPlayerBullet) {
    bulletManager.addBullet(playerX, playerY, isPlayerBullet);
}

void spawnEnemy(EnemyType type) {
    enemyManager.spawnEnemy(type, difficultyMultiplier);
    gameStats.totalEnemiesSpawned++;
}

void spawnBoss() {
    enemyManager.spawnBoss();
    gameStats.bossActive = true;
}