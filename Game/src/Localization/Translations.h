// Translations.h
#ifndef TRANSLATIONS_H
#define TRANSLATIONS_H

#include <vector>
#include <string>
#include "TranslationKeys.h"
#include "Languages.h"

/*
    MENU_GAME,
    MENU_SETTINGS,
    MENU_QUIT,
    AUDIO_VOLUME,
    LANGUAGE_LABEL,
    GAME_SCORE,
    GAME_HEALTH,
    GAME_WAVE,
    GAME_KILLS,
    GAME_BOSS_FIGHT,
    GAME_GAME_OVER,
    GAME_YOU_WIN,
    GAME_PRESS_ESC,
    GAME_RESTART,
    GAME_BACK_TO_MENU
*/
const std::vector<std::vector<std::string>> translations = {
    // English
    {"Game", "Settings", "Quit", "Audio Volume", "Language", 
     "Score", "Health", "Wave", "Kills", "BOSS FIGHT!", 
     "GAME OVER", "YOU WIN!", "Press ESC to return to menu", "Restart", "Back to Menu"},
    // German
    {"Spiel", "Einstellungen", "Beenden", "Lautstärke", "Sprache",
     "Punkte", "Leben", "Welle", "Gegner besiegt", "BOSS KAMPF!",
     "SPIEL VORBEI", "DU GEWINNST!", "Drücke ESC für Hauptmenü", "Neustart", "Zurück zum Menü"}
};

inline std::string getTranslation(TranslationKey key, LanguageCode language) {
    return translations[language][key];
}

#endif