#include "GameWon.h"
#include "Game.h"
#include "GameState.h"
#include "Translations.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

extern int score;
extern GameStats gameStats;
extern int currentWave;
extern LanguageCode selectedLanguage;

void renderGameWonText(SDL_Renderer* renderer, const char* text, float x, float y, TTF_Font* font, SDL_Color color) {
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

void renderGameWonScreen(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 20, 40, 20, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);

    SDL_Color goldColor = {255, 215, 0, 255};
    std::string youWinText = getTranslation(GAME_YOU_WIN, selectedLanguage);
    renderGameWonText(appContext.renderer, youWinText.c_str(), 280, 150, appContext.font, goldColor);

    SDL_Color greenColor = {100, 255, 100, 255};
    std::string congratsText = "Congratulations!";
    if (selectedLanguage == GERMAN) {
        congratsText = "Herzlichen Glückwunsch!";
    }
    renderGameWonText(appContext.renderer, congratsText.c_str(), 250, 200, appContext.font, greenColor);

    // Final score
    SDL_Color whiteColor = {255, 255, 255, 255};
    std::string finalScoreText = getTranslation(GAME_SCORE, selectedLanguage) + ": " + std::to_string(score);
    renderGameWonText(appContext.renderer, finalScoreText.c_str(), 300, 250, appContext.font, whiteColor);

    // Statistics
    std::string killsText = getTranslation(GAME_KILLS, selectedLanguage) + ": " + std::to_string(gameStats.enemiesKilled);
    renderGameWonText(appContext.renderer, killsText.c_str(), 300, 280, appContext.font, whiteColor);

    std::string waveText = getTranslation(GAME_WAVE, selectedLanguage) + ": " + std::to_string(currentWave);
    renderGameWonText(appContext.renderer, waveText.c_str(), 300, 310, appContext.font, whiteColor);

    std::string bossText = "Bosses: " + std::to_string(gameStats.bossesDefeated);
    renderGameWonText(appContext.renderer, bossText.c_str(), 300, 340, appContext.font, whiteColor);

    Uint32 gameTime = (SDL_GetTicks() - gameStats.gameStartTime) / 1000;
    std::string timeText = "Time: " + std::to_string(gameTime / 60) + ":" + 
                          (gameTime % 60 < 10 ? "0" : "") + std::to_string(gameTime % 60);
    if (selectedLanguage == GERMAN) {
        timeText = "Zeit: " + std::to_string(gameTime / 60) + ":" + 
                  (gameTime % 60 < 10 ? "0" : "") + std::to_string(gameTime % 60);
    }
    renderGameWonText(appContext.renderer, timeText.c_str(), 300, 370, appContext.font, whiteColor);

    SDL_Color grayColor = {200, 200, 200, 255};
    std::string restartText = "R - " + getTranslation(GAME_RESTART, selectedLanguage);
    renderGameWonText(appContext.renderer, restartText.c_str(), 300, 430, appContext.font, grayColor);

    std::string menuText = "ESC - " + getTranslation(GAME_BACK_TO_MENU, selectedLanguage);
    renderGameWonText(appContext.renderer, menuText.c_str(), 300, 460, appContext.font, grayColor);

    SDL_RenderPresent(appContext.renderer);
}

void handleGameWonEvents(AppContext& appContext, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_R:
                resetGame();
                appContext.currentState = IN_GAME;
                break;
            case SDL_SCANCODE_ESCAPE:
                appContext.currentState = MAIN_MENU;
                break;
        }
    }
}