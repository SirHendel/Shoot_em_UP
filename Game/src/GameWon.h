#ifndef GAME_WON_H
#define GAME_WON_H

#include "AppContext.h"
#include <SDL3/SDL.h>

void renderGameWonScreen(AppContext& appContext);
void handleGameWonEvents(AppContext& appContext, SDL_Event* event);

#endif