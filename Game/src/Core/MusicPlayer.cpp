#include "MusicPlayer.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>
#include <filesystem>
#include <Core/GameState.h>
static Mix_Music *music = NULL;
const char* oldfile;

void playMusicOnLoop(AppContext& appContext, const char* file) {
    if(oldfile == NULL){
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
        auto audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (!audioDevice) {
            std::cerr << "SDL_OpenAudioDevice Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }

        if (!Mix_OpenAudio(audioDevice, NULL)) {
            std::cerr << "Mix_OpenAudio Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
    }
    switch (appContext.currentState)
        {
        case MAIN_MENU:
            file = "Menu.mp3";
            break;
        case SETTINGS_MENU:
            file = "Retro.mp3";
            break;
        case IN_GAME:
            file = "Hope.mp3";
            break;
        }
    if(file != oldfile){
        Mix_FreeMusic(music);
        oldfile = file;
        auto basePathPtr = SDL_GetBasePath();
        const std::filesystem::path basePath = basePathPtr;
        const auto musicPath = basePath / file;
        /*
        MIX_Track *track MIX_Track * MIX_CreateTrack(MIX_Mixer *mixer);

        SDL_IOStream *stream = SDL_IOFromFile("Boss.mp3","r");

        MIX_SetTrackIOStream(*track,  *stream, true);

        MIX_PlayTrack(*track);
        */
        auto music = Mix_LoadMUS(file);
        if (!music) {
            std::cerr << "Mix_LoadMUS Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
        std::cerr << "Audio_Volumen: " << audioVolume * 128 << std::endl;
        Mix_VolumeMusic(audioVolume* 128);
        Mix_PlayMusic(music,-1);    
    }
}
