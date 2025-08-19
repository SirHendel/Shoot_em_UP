#ifndef SETTINGS_H
#define SETTINGS_H

#include "Core/AppContext.h"

void renderSettingsMenu(AppContext& appContext);
void handleSettingsEvents(AppContext& appContext, SDL_Event* event);

#endif 
