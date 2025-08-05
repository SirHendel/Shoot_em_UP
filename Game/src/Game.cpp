#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <SDL3/SDL.h>
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include "Utils.h"

// Player and bullets
Player player = {400, 500, 15, 15}; // Increased health from 10 to 15
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
int score = 0;
GameStats gameStats = {0, 0, 0, false, 0};
int currentWave = 1;
float difficultyMultiplier = 1.0f;

// Timing variables
Uint64 lastEnemySpawnTime = 0;
const Uint64 basicEnemySpawnDelay = 3000; // Increased from 2000 to 3000 (slower spawn)
Uint64 lastWaveCheckTime = 0;
const Uint64 waveCheckDelay = 15000; // Increased from 10000 to 15000 (slower difficulty increase)

// Boss spawn conditions
const int ENEMIES_FOR_BOSS = 15; // Reduced from 25 to 15 (boss comes sooner)
const int BOSS_HEALTH = 10; // Reduced from 15 to 10 (easier boss)

SDL_Color Enemy::getColor() const {
    switch (type) {
        case BASIC_ENEMY:
            return {255, 0, 0, 255}; // Red
        case HEAVY_ENEMY:
            return {255, 165, 0, 255}; // Orange
        case FAST_ENEMY:
            return {255, 255, 0, 255}; // Yellow
        case SHOOTER_ENEMY:
            return {255, 0, 255, 255}; // Magenta
        case BOSS_ENEMY:
            return {128, 0, 128, 255}; // Purple
        default:
            return {255, 0, 0, 255}; // Default red
    }
}

float Enemy::getSize() const {
    switch (type) {
        case BASIC_ENEMY:
            return 15.0f;
        case HEAVY_ENEMY:
            return 20.0f;
        case FAST_ENEMY:
            return 12.0f;
        case SHOOTER_ENEMY:
            return 18.0f;
        case BOSS_ENEMY:
            return 40.0f;
        default:
            return 15.0f;
    }
}

void initializeGame() {
    srand(static_cast<unsigned int>(time(nullptr)));
    resetGame();
}

void resetGame() {
    player = {400, 500, 15, 15}; // Increased health
    bullets.clear();
    enemies.clear();
    score = 0;
    gameStats = {0, 0, 0, false, SDL_GetTicks()};
    currentWave = 1;
    difficultyMultiplier = 1.0f;
    lastEnemySpawnTime = 0;
    lastWaveCheckTime = SDL_GetTicks();
}

void renderTriangle(SDL_Renderer* renderer, float x, float y, float size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    SDL_Vertex vert[3];
    vert[0] = { {x, y - size}, {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b), static_cast<float>(color.a)} };
    vert[1] = { {x - size, y + size}, {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b), static_cast<float>(color.a)} };
    vert[2] = { {x + size, y + size}, {static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b), static_cast<float>(color.a)} };

    SDL_RenderGeometry(renderer, nullptr, vert, 3, nullptr, 0);
}

void renderHealthBar(SDL_Renderer* renderer, float x, float y, float width, int currentHealth, int maxHealth) {
    if (maxHealth <= 1) return; // Don't show health bar for enemies with 1 max health
    
    // Background (red)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect bgRect = {x - width/2, y - 25, width, 4};
    SDL_RenderFillRect(renderer, &bgRect);
    
    // Health (green)
    if (currentHealth > 0) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        float healthWidth = (static_cast<float>(currentHealth) / maxHealth) * width;
        SDL_FRect healthRect = {x - width/2, y - 25, healthWidth, 4};
        SDL_RenderFillRect(renderer, &healthRect);
    }
}

void renderGame(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);

    // Render player
    SDL_Color playerColor = {0, 255, 0, 255}; // Green
    renderTriangle(appContext.renderer, player.x, player.y, 15, playerColor);

    // Render enemies with different colors and sizes
    for (const auto& enemy : enemies) {
        SDL_Color enemyColor = enemy.getColor();
        float size = enemy.getSize();
        renderTriangle(appContext.renderer, enemy.x, enemy.y, size, enemyColor);
        
        // Render health bar for enemies with more than 1 health
        renderHealthBar(appContext.renderer, enemy.x, enemy.y, size * 2, enemy.health, enemy.maxHealth);
    }

    // Render bullets
    for (const auto& bullet : bullets) {
        if (bullet.isPlayerBullet) {
            SDL_SetRenderDrawColor(appContext.renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); // Yellow for player bullets
        } else {
            SDL_SetRenderDrawColor(appContext.renderer, 255, 100, 100, SDL_ALPHA_OPAQUE); // Light red for enemy bullets
        }
        
        const SDL_FRect bulletRect = {bullet.x - 2, bullet.y - 5, 4, 10};
        SDL_RenderFillRect(appContext.renderer, &bulletRect);
    }

    // Render UI with translations
    SDL_Color textColor = {0, 255, 0, 255};
    
    std::string scoreText = getTranslation(GAME_SCORE, selectedLanguage) + ": " + std::to_string(score);
    renderGameText(appContext.renderer, scoreText.c_str(), 10, 10, appContext.font, textColor);

    std::string healthText = getTranslation(GAME_HEALTH, selectedLanguage) + ": " + std::to_string(player.health);
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

void shootBullet(std::vector<Bullet>& bullets, float playerX, float playerY, bool isPlayerBullet) {
    bullets.emplace_back(playerX, playerY, isPlayerBullet);
}

void spawnEnemy(EnemyType type) {
    float x = static_cast<float>(rand() % 750 + 25); // Keep enemies on screen
    float speed, health, scoreValue;
    
    switch (type) {
        case BASIC_ENEMY:
            speed = 0.8f * difficultyMultiplier; // Reduced from 1.0f
            health = 1;
            scoreValue = 10;
            break;
        case HEAVY_ENEMY:
            speed = 0.3f * difficultyMultiplier; // Reduced from 0.5f
            health = 2; // Reduced from 3
            scoreValue = 25;
            break;
        case FAST_ENEMY:
            speed = 1.8f * difficultyMultiplier; // Reduced from 2.5f
            health = 1;
            scoreValue = 15;
            break;
        case SHOOTER_ENEMY:
            speed = 0.5f * difficultyMultiplier; // Reduced from 0.8f
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
    gameStats.totalEnemiesSpawned++;
}

void spawnBoss() {
    float x = 400; // Center of screen
    enemies.emplace_back(x, -50, BOSS_HEALTH, 0.5f, 100, BOSS_ENEMY);
    gameStats.bossActive = true;
}

void handleGameEvents(AppContext& appContext, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_LEFT:
                if (player.x > 15) player.x -= 15;
                break;
            case SDL_SCANCODE_RIGHT:
                if (player.x < 785) player.x += 15;
                break;
            case SDL_SCANCODE_SPACE:
                shootBullet(bullets, player.x, player.y);
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
        difficultyMultiplier += 0.1f; // Reduced from 0.2f (slower difficulty increase)
        lastWaveCheckTime = currentTime;
    }
}

void checkGameOverConditions(AppContext& appContext) {
    if (player.health <= 0) {
        appContext.currentState = GAME_OVER;
        return;
    }
    
    // Win condition: defeat 2 bosses (reduced from 3)
    if (gameStats.bossesDefeated >= 2) {
        appContext.currentState = GAME_WON;
        return;
    }
}

void updateGameState(AppContext& appContext) {
    Uint64 currentTime = SDL_GetTicks();
    
    // Update difficulty over time
    updateDifficulty();
    
    // Check for boss spawn
    if (!gameStats.bossActive && gameStats.enemiesKilled > 0 && gameStats.enemiesKilled % ENEMIES_FOR_BOSS == 0) {
        // Clear existing enemies before spawning boss
        enemies.clear();
        spawnBoss();
    }
    
    // Spawn regular enemies if no boss is active
    if (!gameStats.bossActive && currentTime - lastEnemySpawnTime > basicEnemySpawnDelay / difficultyMultiplier) {
        // Choose enemy type based on wave
        EnemyType typeToSpawn = BASIC_ENEMY;
        int randChoice = rand() % 100;
        
        if (currentWave >= 2) {
            if (randChoice < 15) typeToSpawn = HEAVY_ENEMY; // Reduced from 20
            else if (randChoice < 30) typeToSpawn = FAST_ENEMY; // Reduced from 40
            else if (randChoice < 40 && currentWave >= 3) typeToSpawn = SHOOTER_ENEMY; // Reduced from 55
        }
        
        spawnEnemy(typeToSpawn);
        lastEnemySpawnTime = currentTime;
    }

    // Update enemy positions and behavior
    for (auto& enemy : enemies) {
        enemy.y += enemy.speed;
        
        // Shooter enemies shoot at player (less frequently)
        if (enemy.type == SHOOTER_ENEMY && currentTime - enemy.lastShotTime > 3000) { // Increased from 2000
            if (enemy.y > 50 && enemy.y < 400) { // Only shoot when in good range
                shootBullet(bullets, enemy.x, enemy.y + 20, false);
                enemy.lastShotTime = currentTime;
            }
        }
        
        // Boss shooting pattern (less frequently)
        if (enemy.type == BOSS_ENEMY && currentTime - enemy.lastShotTime > 2500) { // Increased from 1500
            // Shoot fewer bullets
            for (int i = -1; i <= 1; i++) { // Reduced from -2 to 2
                shootBullet(bullets, enemy.x + i * 25, enemy.y + 40, false);
            }
            enemy.lastShotTime = currentTime;
        }
    }

    // Update bullet positions
    for (auto& bullet : bullets) {
        if (bullet.isPlayerBullet) {
            bullet.y -= 8; // Player bullets move faster
        } else {
            bullet.y += 4; // Enemy bullets
        }
    }

    // Handle bullet-enemy collisions (only player bullets)
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool bulletRemoved = false;
        
        if (bulletIt->isPlayerBullet) {
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
                float dx = bulletIt->x - enemyIt->x;
                float dy = bulletIt->y - enemyIt->y;
                float distance = std::sqrt(dx * dx + dy * dy);
                float hitRadius = enemyIt->getSize();

                if (distance < hitRadius) {
                    enemyIt->health -= 1;
                    bulletIt = bullets.erase(bulletIt);
                    bulletRemoved = true;
                    
                    if (enemyIt->health <= 0) {
                        score += enemyIt->score;
                        gameStats.enemiesKilled++;
                        
                        if (enemyIt->type == BOSS_ENEMY) {
                            gameStats.bossesDefeated++;
                            gameStats.bossActive = false;
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

    // Handle enemy bullets hitting player
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        if (!bulletIt->isPlayerBullet) {
            float dx = bulletIt->x - player.x;
            float dy = bulletIt->y - player.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            if (distance < 20) {
                player.health -= 1;
                bulletIt = bullets.erase(bulletIt);
                continue;
            }
        }
        ++bulletIt;
    }

    // Handle enemy-player collisions
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        float dx = player.x - enemyIt->x;
        float dy = player.y - enemyIt->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 25) {
            player.health -= (enemyIt->type == BOSS_ENEMY) ? 3 : 1;
            enemyIt = enemies.erase(enemyIt);
        } else {
            ++enemyIt;
        }
    }

    // Remove enemies that have passed the player (damage player)
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        if (enemyIt->y > 600) {
            player.health -= (enemyIt->type == BOSS_ENEMY) ? 2 : 1;
            enemyIt = enemies.erase(enemyIt);
        } else {
            ++enemyIt;
        }
    }

    // Remove off-screen bullets
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
        return bullet.y < -10 || bullet.y > 610;
    }), bullets.end());

    // Check game over conditions
    checkGameOverConditions(appContext);
}