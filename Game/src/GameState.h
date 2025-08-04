#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <Languages.h>

enum GameState {
    MAIN_MENU,
    SETTINGS_MENU,
    IN_GAME,
    QUIT_GAME,
    GAME_OVER,
    GAME_WON
};

extern LanguageCode selectedLanguage;
extern float audioVolume;


#endif 
