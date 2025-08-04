#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "AppContext.h"
#include <SDL3/SDL.h>

void renderGameOverScreen(AppContext& appContext);
void handleGameOverEvents(AppContext& appContext, SDL_Event* event);

#endif