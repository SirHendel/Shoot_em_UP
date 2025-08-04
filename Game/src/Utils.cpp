#include "Utils.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>
#include <filesystem>
#include "MusicPlayer.h"

bool initializeSDL(AppContext& appContext) {
    if (not SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error initializing SDL: %s", SDL_GetError());
        return false;
    }

    if (not TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error initializing TTF: %s", SDL_GetError());
        return false;
    }

    appContext.window = SDL_CreateWindow("SDL Menu Sample", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!appContext.window) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error creating window: %s", SDL_GetError());
        return false;
    }

    appContext.renderer = SDL_CreateRenderer(appContext.window, nullptr);
    if (!appContext.renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error creating renderer: %s", SDL_GetError());
        return false;
    }

    auto basePathPtr = SDL_GetBasePath();
    if (!basePathPtr) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error getting base path: %s", SDL_GetError());
        return false;
    }

    std::filesystem::path basePath = basePathPtr;
    auto fontPath = basePath / "Inter-VariableFont.ttf";
    appContext.font = TTF_OpenFont(fontPath.string().c_str(), 36);
    if (!appContext.font) {
        SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error loading font: %s", SDL_GetError());
        return false;
    }
    playMusicOnLoop(appContext,"");
    return true;
}

void cleanupSDL(AppContext& appContext) {
    if (appContext.font) {
        TTF_CloseFont(appContext.font);
    }
    if (appContext.renderer) {
        SDL_DestroyRenderer(appContext.renderer);
    }
    if (appContext.window) {
        SDL_DestroyWindow(appContext.window);
    }
    if (appContext.audioDevice){
        Mix_CloseAudio();
    }
    TTF_Quit();
    SDL_Quit();
}
