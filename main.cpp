#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "level.h"
#include "game_loop.h"

using namespace std;

int main(int argc, char* argv[])
{

    Uint32 lastTick = SDL_GetTicks();
    int currentLevel = 0, remainingTime = gameLevels[currentLevel].timelimit, score = 0, stoplevel;
    int clickGoiy = 6;

    Graphics graphics;
    graphics.init();

    Pikachu pikachu;
    pikachu.init(gameLevels[currentLevel].imgcount);
    graphics.loadAllTextures();

    Audio audio;
    audio.init();

    TTF_Font* font = graphics.loadFont("assets/ttf/dlxfont_.ttf", 25);
    TTF_Font* font_menu = graphics.loadFont("assets/ttf/dlxfont_.ttf", 30);

    std::vector<Button*> buttons;
    graphics.initButton(graphics.renderer, buttons);

    GameState gamestate = MENU;

    bool isPause = false, isMuted = false;
    bool running = true, isPracticeMode = false, isFlashing = false;
    SDL_Event e;
    bool menu = true;

    while (running) {
        if(gamestate == MENU){
            audio.play(audio.music);
            while(SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT) running = false;
                handleMenuEvents(e, gamestate, running, font_menu, audio);
            }
            renderMenuScreen(graphics, font_menu, gamestate);
        }
        if(gamestate == GAME){
            Mix_HaltMusic();
            Timeupdate(lastTick, remainingTime, isPause, currentLevel, pikachu, isPracticeMode, isFlashing, gamestate);
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) running = false;
                handleEvents(e, buttons, graphics, pikachu, score, isMuted, isPause, currentLevel, remainingTime, isPracticeMode, clickGoiy, audio, stoplevel);
            }

            if (!pikachu.isclear() && !pikachu.hasmove()) pikachu.shuffle();
            else if (pikachu.isclear()) handleWin(graphics, pikachu, currentLevel, remainingTime, clickGoiy, audio, gamestate);

            renderScene(graphics, pikachu, score, remainingTime, font, isPause, isMuted, isPracticeMode, buttons, clickGoiy, currentLevel, isFlashing);
        }else if(gamestate == LOSE) {
            audio.play(audio.music);
            while(SDL_PollEvent(&e)){
                if (e.type == SDL_QUIT) running = false;
                handleLoseEvents(e, gamestate, running, font_menu, audio, clickGoiy);
            }
            renderLoseScreen(graphics, font_menu, gamestate);
        }
        SDL_Delay(100);
    }

    graphics.quit(buttons, font, audio);
    SDL_Quit();
    return 0;
}
