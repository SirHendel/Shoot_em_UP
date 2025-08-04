#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <SDL3/SDL.h>
#include <algorithm>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <cmath>

Player player = {400, 500, 15, 15};
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
int score = 0;
GameStats gameStats = {0, 0, 0, false, 0};
int currentWave = 1;
float difficultyMultiplier = 1.0f;

Uint32 lastEnemySpawnTime = 0;
const Uint32 basicEnemySpawnDelay = 3000;
Uint32 lastWaveCheckTime = 0;
const Uint32 waveCheckDelay = 15000; 

const int ENEMIES_FOR_BOSS = 15; 
const int BOSS_HEALTH = 10; 

SDL_Color Enemy::getColor() const {
    switch (type) {
        case BASIC_ENEMY:
            return {255, 0, 0, 255}; 
        case HEAVY_ENEMY:
            return {255, 165, 0, 255}; 
        case FAST_ENEMY:
            return {255, 255, 0, 255}; 
        case SHOOTER_ENEMY:
            return {255, 0, 255, 255}; 
        case BOSS_ENEMY:
            return {128, 0, 128, 255}; 
        default:
            return {255, 0, 0, 255}; 
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
    player = {400, 500, 15, 15}; 
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
    vert[0] = { {x, y - size}, {color.r, color.g, color.b, color.a} };
    vert[1] = { {x - size, y + size}, {color.r, color.g, color.b, color.a} };
    vert[2] = { {x + size, y + size}, {color.r, color.g, color.b, color.a} };

    SDL_RenderGeometry(renderer, nullptr, vert, 3, nullptr, 0);
}

void renderHealthBar(SDL_Renderer* renderer, float x, float y, float width, int currentHealth, int maxHealth) {
    if (maxHealth <= 1) return;
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect bgRect = {x - width/2, y - 25, width, 4};
    SDL_RenderFillRect(renderer, &bgRect);
    
    if (currentHealth > 0) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        float healthWidth = (static_cast<float>(currentHealth) / maxHealth) * width;
        SDL_FRect healthRect = {x - width/2, y - 25, healthWidth, 4};
        SDL_RenderFillRect(renderer, &healthRect);
    }
}

void renderGameText(SDL_Renderer* renderer, const char* text, float x, float y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, strlen(text), color);
    if (!surface) {
        std::cerr << "Failed to render text: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect text_rect = {x, y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, nullptr, &text_rect);

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void renderGame(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);

    SDL_Color playerColor = {0, 255, 0, 255}; 
    renderTriangle(appContext.renderer, player.x, player.y, 15, playerColor);

    for (const auto& enemy : enemies) {
        SDL_Color enemyColor = enemy.getColor();
        float size = enemy.getSize();
        renderTriangle(appContext.renderer, enemy.x, enemy.y, size, enemyColor);
        
        renderHealthBar(appContext.renderer, enemy.x, enemy.y, size * 2, enemy.health, enemy.maxHealth);
    }

    for (const auto& bullet : bullets) {
        if (bullet.isPlayerBullet) {
            SDL_SetRenderDrawColor(appContext.renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); 
        } else {
            SDL_SetRenderDrawColor(appContext.renderer, 255, 100, 100, SDL_ALPHA_OPAQUE);
        }
        
        const SDL_FRect bulletRect = {bullet.x - 2, bullet.y - 5, 4, 10};
        SDL_RenderFillRect(appContext.renderer, &bulletRect);
    }

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

    SDL_Color instructionColor = {200, 200, 200, 255};
    std::string escText = getTranslation(GAME_PRESS_ESC, selectedLanguage);
    renderGameText(appContext.renderer, escText.c_str(), 10, 550, appContext.font, instructionColor);

    SDL_RenderPresent(appContext.renderer);
}

void shootBullet(std::vector<Bullet>& bullets, float playerX, float playerY, bool isPlayerBullet) {
    bullets.emplace_back(playerX, playerY, isPlayerBullet);
}

void spawnEnemy(EnemyType type) {
    float x = static_cast<float>(rand() % 750 + 25); 
    float speed, health, scoreValue;
    
    switch (type) {
        case BASIC_ENEMY:
            speed = 0.8f * difficultyMultiplier; 
            health = 1;
            scoreValue = 10;
            break;
        case HEAVY_ENEMY:
            speed = 0.3f * difficultyMultiplier; 
            health = 3;
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
    gameStats.totalEnemiesSpawned++;
}

void spawnBoss() {
    float x = 400;
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
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastWaveCheckTime > waveCheckDelay) {
        currentWave++;
        difficultyMultiplier += 0.1f;
        lastWaveCheckTime = currentTime;
    }
}

void checkGameOverConditions(AppContext& appContext) {
    if (player.health <= 0) {
        appContext.currentState = GAME_OVER;
        return;
    }
    
    if (gameStats.bossesDefeated >= 2) {
        appContext.currentState = GAME_WON;
        return;
    }
}

void updateGameState(AppContext& appContext) {
    Uint32 currentTime = SDL_GetTicks();
    
    updateDifficulty();
    
    if (!gameStats.bossActive && gameStats.enemiesKilled > 0 && gameStats.enemiesKilled % ENEMIES_FOR_BOSS == 0) {
        enemies.clear();
        spawnBoss();
    }
    
    if (!gameStats.bossActive && currentTime - lastEnemySpawnTime > basicEnemySpawnDelay / difficultyMultiplier) {
        EnemyType typeToSpawn = BASIC_ENEMY;
        int randChoice = rand() % 100;
        
        if (currentWave >= 2) {
            if (randChoice < 15) typeToSpawn = HEAVY_ENEMY; 
            else if (randChoice < 30) typeToSpawn = FAST_ENEMY; 
            else if (randChoice < 40 && currentWave >= 3) typeToSpawn = SHOOTER_ENEMY; 
        }
        
        spawnEnemy(typeToSpawn);
        lastEnemySpawnTime = currentTime;
    }

    for (auto& enemy : enemies) {
        enemy.y += enemy.speed;
        
        if (enemy.type == SHOOTER_ENEMY && currentTime - enemy.lastShotTime > 3000) { 
            if (enemy.y > 50 && enemy.y < 400) { 
                shootBullet(bullets, enemy.x, enemy.y + 20, false);
                enemy.lastShotTime = currentTime;
            }
        }
        
        if (enemy.type == BOSS_ENEMY && currentTime - enemy.lastShotTime > 2500) { 
            for (int i = -1; i <= 1; i++) { 
                shootBullet(bullets, enemy.x + i * 25, enemy.y + 40, false);
            }
            enemy.lastShotTime = currentTime;
        }
    }

    for (auto& bullet : bullets) {
        if (bullet.isPlayerBullet) {
            bullet.y -= 8; 
        } else {
            bullet.y += 4; 
        }
    }

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

    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
        if (enemyIt->y > 600) {
            player.health -= (enemyIt->type == BOSS_ENEMY) ? 2 : 1;
            enemyIt = enemies.erase(enemyIt);
        } else {
            ++enemyIt;
        }
    }

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
        return bullet.y < -10 || bullet.y > 610;
    }), bullets.end());

    checkGameOverConditions(appContext);
}