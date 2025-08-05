#include "Utils.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>
#include <filesystem>
#include "MusicPlayer.h"
#include <iostream>

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


void renderGameText(SDL_Renderer* renderer, const char* text, float x, float y, TTF_Font* font, SDL_Color color) {
    // Safety checks
    if (!text || strlen(text) == 0) {
        std::cerr << "Warning: Trying to render empty text" << std::endl;
        return;
    }
    
    if (!font) {
        std::cerr << "Error: Font is null" << std::endl;
        return;
    }
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, static_cast<size_t>(strlen(text)), color);
    if (!surface) {
        std::cerr << "Failed to render text '" << text << "': " << SDL_GetError() << std::endl;
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