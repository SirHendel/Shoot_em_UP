#ifndef GAME_H
#define GAME_H

#include "Core/AppContext.h"
#include "Core/GameState.h"
#include "Localization/Translations.h"
#include "Game/Entities/Enemy.h" // EnemyType is now in Enemy.h
#include <vector>
#include <SDL3/SDL.h>



// Game statistics
struct GameStats {
    int enemiesKilled;
    int bossesDefeated;
    int totalEnemiesSpawned;
    bool bossActive;
    Uint64 gameStartTime;
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
void checkGameOverConditions(AppContext& appContext);
void updateDifficulty();

#endif