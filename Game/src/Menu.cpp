#include "Menu.h"
#include "TranslationKeys.h"
#include "Translations.h"
#include "Languages.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3_ttf/SDL_ttf.h>
#include "MusicPlayer.h"

const int menuItemCount = 3;
int selectedItem = 0;

void renderText(SDL_Renderer* renderer, const char* text, float x, float y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, strlen(text), color);
    if (!surface) {
        printf("Failed to render text: %s\n", SDL_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_DestroySurface(surface);
        return;
    }
    SDL_FRect text_rect = {x, y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, nullptr, &text_rect);
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
}

void renderMainMenu(AppContext& appContext) {
    SDL_SetRenderDrawColor(appContext.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(appContext.renderer);

    SDL_Color Green = {0, 255, 0, 255};
    SDL_Color White = {255, 255, 255, 255};

    for (int i = 0; i < menuItemCount; ++i) {
        float yPos = 200 + i * 100;
        SDL_Color color = (i == selectedItem) ? White : Green;

        const char* menuItemText;
        switch (i) {
            case 0: // Game
                menuItemText = getTranslation(MENU_GAME, selectedLanguage).c_str();
                break;
            case 1: // Settings
                menuItemText = getTranslation(MENU_SETTINGS, selectedLanguage).c_str();
                break;
            case 2: // Quit
                menuItemText = getTranslation(MENU_QUIT, selectedLanguage).c_str();
                break;
            default:
                menuItemText = "";
                break;
        }

        renderText(appContext.renderer, menuItemText, 400 - 50, yPos, appContext.font, color);
    }

    SDL_RenderPresent(appContext.renderer);
}

void handleMenuEvents(AppContext& appContext, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_UP:
                selectedItem = (selectedItem - 1 + menuItemCount) % menuItemCount; 
                break;
            case SDL_SCANCODE_DOWN:
                selectedItem = (selectedItem + 1) % menuItemCount; 
                break;
            case SDL_SCANCODE_ESCAPE:
                appContext.currentState = QUIT_GAME;
                break;
            case SDL_SCANCODE_RETURN:
                switch (selectedItem) {
                    case 0: 
                        appContext.currentState = IN_GAME;
                        playMusicOnLoop(appContext,"");
                        break;
                    case 1: 
                        appContext.currentState = SETTINGS_MENU;
                        playMusicOnLoop(appContext,"");
                        break;
                    case 2: 
                        appContext.currentState = QUIT_GAME;
                        break;
                }
                break;
        }
    }
}
