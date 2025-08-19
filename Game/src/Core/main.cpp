#include <SDL3/SDL.h>
#include "AppContext.h"
#include "Utils.h"
#include "GameState.h"
#include "UI/Menu.h"
#include "UI/Settings.h"
#include "Game/Game.h"
#include "UI/GameOver.h"
#include "UI/GameWon.h"
#include "Core/MusicPlayer.h"

LanguageCode selectedLanguage = ENGLISH;
float audioVolume = 0.75f;

int main(int argc, char* argv[]) {
    AppContext appContext{};
    if (!initializeSDL(appContext)) {
        cleanupSDL(appContext);
        return -1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    initializeGame();

    bool running = true;
    const Uint32 frameDelay = 1000 / 60; 

    while (running) {
        Uint32 frameStart = SDL_GetTicks(); 

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            switch (appContext.currentState) {
                case MAIN_MENU:
                    handleMenuEvents(appContext, &event);
                    break;
                case SETTINGS_MENU:
                    handleSettingsEvents(appContext, &event);
                    break;
                case IN_GAME:
                    handleGameEvents(appContext, &event);
                    break;
                case GAME_OVER:
                    handleGameOverEvents(appContext, &event);
                    break;
                case GAME_WON:
                    handleGameWonEvents(appContext, &event);
                    break;
                case QUIT_GAME:
                    running = false;
                    break;
                default:
                    running = false;
                    break;
            }
        }

        switch (appContext.currentState) {
            case MAIN_MENU:
                renderMainMenu(appContext);
                break;
            case SETTINGS_MENU:
                renderSettingsMenu(appContext);
                break;
            case IN_GAME:
                updateGameState(appContext);
                renderGame(appContext);
                break;
            case GAME_OVER:
                renderGameOverScreen(appContext);
                break;
            case GAME_WON:
                renderGameWonScreen(appContext);
                break;
            default:
                running = false;
                break;
        }

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    cleanupSDL(appContext);
    return 0;
}