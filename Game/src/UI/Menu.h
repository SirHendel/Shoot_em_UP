#ifndef MENU_H
#define MENU_H

#include "Core/AppContext.h"

void renderMainMenu(AppContext& appContext);
void handleMenuEvents(AppContext& appContext, SDL_Event* event);

#endif 
