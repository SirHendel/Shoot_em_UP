#include "GameWon.h"
#include "Game.h"
#include "GameState.h"
#include "Translations.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "Utils.h"

// External variables from Game.cpp
extern int score;
extern GameStats gameStats;
extern int currentWave;
extern LanguageCode selectedLanguage;

void renderGameWonScreen(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 20, 40, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);

    // You Win title
    SDL_Color goldColor = {255, 215, 0, 255};
    std::string youWinText = getTranslation(GAME_YOU_WIN, selectedLanguage);
    renderGameText(appContext.renderer, youWinText.c_str(), 280, 150, appContext.font, goldColor);

    // Congratulations message
    SDL_Color greenColor = {100, 255, 100, 255};
    std::string congratsText = "Congratulations!";
    if (selectedLanguage == GERMAN) {
        congratsText = "Herzlichen Glückwunsch!";
    }
    renderGameText(appContext.renderer, congratsText.c_str(), 250, 200, appContext.font, greenColor);

    // Final score
    SDL_Color whiteColor = {255, 255, 255, 255};
    std::string finalScoreText = getTranslation(GAME_SCORE, selectedLanguage) + ": " + std::to_string(score);
    renderGameText(appContext.renderer, finalScoreText.c_str(), 300, 250, appContext.font, whiteColor);

    // Statistics
    std::string killsText = getTranslation(GAME_KILLS, selectedLanguage) + ": " + std::to_string(gameStats.enemiesKilled);
    renderGameText(appContext.renderer, killsText.c_str(), 300, 280, appContext.font, whiteColor);

    std::string waveText = getTranslation(GAME_WAVE, selectedLanguage) + ": " + std::to_string(currentWave);
    renderGameText(appContext.renderer, waveText.c_str(), 300, 310, appContext.font, whiteColor);

    std::string bossText = "Bosses: " + std::to_string(gameStats.bossesDefeated);
    renderGameText(appContext.renderer, bossText.c_str(), 300, 340, appContext.font, whiteColor);

    // Game completion time
    Uint32 gameTime = (SDL_GetTicks() - gameStats.gameStartTime) / 1000;
    std::string timeText = "Time: " + std::to_string(gameTime / 60) + ":" + 
                          (gameTime % 60 < 10 ? "0" : "") + std::to_string(gameTime % 60);
    if (selectedLanguage == GERMAN) {
        timeText = "Zeit: " + std::to_string(gameTime / 60) + ":" + 
                  (gameTime % 60 < 10 ? "0" : "") + std::to_string(gameTime % 60);
    }
    renderGameText(appContext.renderer, timeText.c_str(), 300, 370, appContext.font, whiteColor);

    // Instructions
    SDL_Color grayColor = {200, 200, 200, 255};
    std::string restartText = "R - " + getTranslation(GAME_RESTART, selectedLanguage);
    renderGameText(appContext.renderer, restartText.c_str(), 300, 430, appContext.font, grayColor);

    std::string menuText = "ESC - " + getTranslation(GAME_BACK_TO_MENU, selectedLanguage);
    renderGameText(appContext.renderer, menuText.c_str(), 300, 460, appContext.font, grayColor);

    SDL_RenderPresent(appContext.renderer);
}

void handleGameWonEvents(AppContext& appContext, SDL_Event* event) {
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