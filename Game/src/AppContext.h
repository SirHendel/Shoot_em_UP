#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <SDL3/SDL.h>
#include <GameState.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cmath>
#include <string_view>
#include <filesystem>

struct AppContext {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    GameState currentState;
    SDL_AudioDeviceID audioDevice;
    
};

#endif
