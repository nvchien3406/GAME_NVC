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
    int currentLevel = 0, remainingTime = gameLevels[currentLevel].timelimit, score = 0;

    Graphics graphics;
    graphics.init();

    Pikachu pikachu;
    pikachu.init(gameLevels[currentLevel].imgcount);
    graphics.loadAllTextures();

    TTF_Font* font = graphics.loadFont("assets/ttf/dlxfont_.ttf", 25);
    //TTF_Font* font1 = graphics.loadFont("assets/dlxfont_.ttf", 40);

    std::vector<Button*> buttons;
    Goiy* goiy;
    graphics.initButton(graphics.renderer, buttons, goiy);

    bool isPause = false, isMuted = false;
    bool running = true, isPracticeMode = false;
    SDL_Event e;

    while (running) {
        Timeupdate(lastTick, remainingTime, isPause, currentLevel, pikachu, isPracticeMode);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            handleEvents(e, buttons, goiy, graphics, pikachu, score, isMuted, isPause, currentLevel, remainingTime, isPracticeMode);
        }

        if (!pikachu.isclear() && !pikachu.hasmove()) pikachu.xaotron();
        else if (pikachu.isclear()) handleWin(graphics, pikachu, currentLevel, remainingTime, goiy);

        renderScene(graphics, pikachu, score, remainingTime, font, isPause, isMuted, isPracticeMode, buttons, goiy, currentLevel);
        SDL_Delay(100);
    }

    graphics.quit(buttons, goiy, font);
    SDL_Quit();
    return 0;
}
