#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <SDL_mixer.h>
class Audio{
public:
    Mix_Music* gMusic = nullptr;
    Mix_Chunk* click = nullptr;
    Mix_Chunk* linked = nullptr;
    Mix_Chunk* win = nullptr;
    Mix_Chunk* oho = nullptr;
    Mix_Chunk* mouse = nullptr;
    Mix_Music* music = nullptr;


    Mix_Music *loadMusic(const char* path)
    {
        Mix_Music *gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                           "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
    }
    void play(Mix_Music *gMusic)
    {
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
    }

    Mix_Chunk* loadSound(const char* path) {
        Mix_Chunk* gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                       "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        return gChunk;
    }
    void play(Mix_Chunk* gChunk) {
        if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0 );
        }
    }

    void init(){
        loadsoundeffect();
    }
    void loadsoundeffect(){
        click = loadSound("Sound/click.mp3");
        linked = loadSound("Sound/linked.mp3");
        win = loadSound("Sound/win.mp3");
        oho = loadSound("Sound/oho.mp3");
        mouse = loadSound("Sound/mouse-click.mp3");
        music = loadMusic("Sound/music.mp3");
    }

    void quit(){
        if (gMusic != nullptr) {
            Mix_FreeMusic(gMusic);
            gMusic = nullptr;
        }

        if (click != nullptr) {
            Mix_FreeChunk(click);
            click = nullptr;
        }

        if (linked != nullptr) {
            Mix_FreeChunk(linked);
            linked = nullptr;
        }

        if (win != nullptr) {
            Mix_FreeChunk(win);
            win = nullptr;
        }

        if (oho != nullptr) {
            Mix_FreeChunk(oho);
            oho = nullptr;
        }
        if (mouse != nullptr) {
            Mix_FreeChunk(mouse);
            mouse = nullptr;
        }
        if(music != nullptr){
            Mix_FreeMusic(music);
            music = nullptr;
        }

        Mix_Quit();
    }
};

#endif // AUDIO_H_INCLUDED
