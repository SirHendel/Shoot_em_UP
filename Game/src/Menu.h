#ifndef MENU_H
#define MENU_H

#include "AppContext.h"

void renderMainMenu(AppContext& appContext);
void handleMenuEvents(AppContext& appContext, SDL_Event* event);

#endif 
