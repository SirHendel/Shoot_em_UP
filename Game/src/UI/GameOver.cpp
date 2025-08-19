#include "GameOver.h"
#include "Game/Game.h"
#include "Core/GameState.h"
#include "Localization/Translations.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "Core/Utils.h"
// External variables from Game.cpp
extern int score;
extern GameStats gameStats;
extern int currentWave;
extern LanguageCode selectedLanguage;

void renderGameOverScreen(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 20, 20, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);

    // Game Over title
    SDL_Color redColor = {255, 50, 50, 255};
    std::string gameOverText = getTranslation(GAME_GAME_OVER, selectedLanguage);
    renderGameText(appContext.renderer, gameOverText.c_str(), 250, 150, appContext.font, redColor);

    // Final score
    SDL_Color whiteColor = {255, 255, 255, 255};
    std::string finalScoreText = getTranslation(GAME_SCORE, selectedLanguage) + ": " + std::to_string(score);
    renderGameText(appContext.renderer, finalScoreText.c_str(), 300, 220, appContext.font, whiteColor);

    // Statistics
    std::string killsText = getTranslation(GAME_KILLS, selectedLanguage) + ": " + std::to_string(gameStats.enemiesKilled);
    renderGameText(appContext.renderer, killsText.c_str(), 300, 250, appContext.font, whiteColor);

    std::string waveText = getTranslation(GAME_WAVE, selectedLanguage) + ": " + std::to_string(currentWave);
    renderGameText(appContext.renderer, waveText.c_str(), 300, 280, appContext.font, whiteColor);

    std::string bossText = "Bosses: " + std::to_string(gameStats.bossesDefeated);
    renderGameText(appContext.renderer, bossText.c_str(), 300, 310, appContext.font, whiteColor);

    // Instructions
    SDL_Color grayColor = {200, 200, 200, 255};
    std::string restartText = "R - " + getTranslation(GAME_RESTART, selectedLanguage);
    renderGameText(appContext.renderer, restartText.c_str(), 300, 380, appContext.font, grayColor);

    std::string menuText = "ESC - " + getTranslation(GAME_BACK_TO_MENU, selectedLanguage);
    renderGameText(appContext.renderer, menuText.c_str(), 300, 410, appContext.font, grayColor);

    SDL_RenderPresent(appContext.renderer);
}

void handleGameOverEvents(AppContext& appContext, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_R:
                // Restart game
                resetGame();
                appContext.currentState = IN_GAME;
                break;
            case SDL_SCANCODE_ESCAPE:
                // Back to main menu
                appContext.currentState = MAIN_MENU;
                break;
        }
    }
}