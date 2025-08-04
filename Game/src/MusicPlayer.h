#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "AppContext.h"

void playMusicOnLoop(AppContext& appContext, const char* filePath);

#endif
