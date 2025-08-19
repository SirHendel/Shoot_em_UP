#ifndef UTILS_H
#define UTILS_H

#include "AppContext.h"

bool initializeSDL(AppContext& appContext);
void cleanupSDL(AppContext& appContext);
void renderGameText(SDL_Renderer* renderer, const char* text, float x, float y, TTF_Font* font, SDL_Color color);

#endif 
