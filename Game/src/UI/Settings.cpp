#include "Settings.h"
#include "Localization/TranslationKeys.h"
#include "Localization/Translations.h"
#include "Localization/Languages.h"
#include "Core/GameState.h"
#include "Core/MusicPlayer.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>

const std::vector<std::string> languages = {"English", "German"};
const int menuItemCount = 2; 
int selectedSettingsItem = 0;

void renderTextSettings(SDL_Renderer* renderer, const char* text, float x, float y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, strlen(text), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect text_rect = {x + 5, y + 5, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, nullptr, &text_rect);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void renderAudioScale(SDL_Renderer* renderer, float x, float y, float width, float height, float volume, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 10, 255, 10, SDL_ALPHA_OPAQUE);
    
    const SDL_FRect scaleBackground = {x, y, width, height};
    SDL_RenderRect(renderer, &scaleBackground);
    SDL_SetRenderDrawColor(renderer, 10, 255, 10, SDL_ALPHA_OPAQUE);
    
    const SDL_FRect scaleFill = {x, y, (width * volume), height};
    SDL_RenderRect(renderer, &scaleFill);
    SDL_RenderFillRect(renderer, &scaleFill);
    
    SDL_Color White = {255, 255, 255, 255};
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color color =  selectedSettingsItem ? White : Green;
    renderTextSettings(renderer, getTranslation(AUDIO_VOLUME, selectedLanguage).c_str(), x, y - 40, font, color);
}

void renderLanguageDropdown(SDL_Renderer* renderer, float x, float y, int selected, TTF_Font* font) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    const SDL_FRect dropdownBackground = {x, y, 300, 100};
    SDL_RenderRect(renderer, &dropdownBackground);
    
    SDL_Color White = {255, 255, 255, 255};
    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color color =  selectedSettingsItem ? Green : White;
    renderTextSettings(renderer, languages[selected].c_str(), x, y + 30, font, Green);


    renderTextSettings(renderer, getTranslation(LANGUAGE_LABEL, selectedLanguage).c_str(), x, y, font, color);
}

void renderSettingsMenu(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);
    // Render audio scale
    renderAudioScale(appContext.renderer, 300, 200, 200, 20, audioVolume, appContext.font);
    // Render language dropdown
    renderLanguageDropdown(appContext.renderer, 300, 300, selectedLanguage, appContext.font);
    SDL_RenderPresent(appContext.renderer);
}

void handleSettingsEvents(AppContext& appContext, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_LEFT:
                if (selectedSettingsItem == 0) {
                    selectedLanguage = static_cast<LanguageCode>((selectedLanguage + 1) % LANGUAGE_COUNT);
                } else if (selectedSettingsItem == 1) {
                    audioVolume = std::max(0.0f, audioVolume - 0.1f);
                    Mix_VolumeMusic(audioVolume*128);
                } 
                break;
            case SDL_SCANCODE_RIGHT:
                if (selectedSettingsItem == 0) {
                    selectedLanguage = static_cast<LanguageCode>((selectedLanguage - 1 + LANGUAGE_COUNT) % LANGUAGE_COUNT);
                } else if (selectedSettingsItem == 1) {
                    audioVolume = std::min(1.0f, audioVolume + 0.1f); 
                    Mix_VolumeMusic(audioVolume*128);
                }
                break;
            case SDL_SCANCODE_UP:
                selectedSettingsItem = (selectedSettingsItem - 1 + menuItemCount) % menuItemCount; 
                break;
            case SDL_SCANCODE_DOWN:
                selectedSettingsItem = (selectedSettingsItem + 1) % menuItemCount; 
                break;
            case SDL_SCANCODE_ESCAPE:
                appContext.currentState = MAIN_MENU;
                break;
        }
    }
}

