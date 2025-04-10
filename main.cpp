#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "graphics.h"
#include "logic.h"
#include "button.h"
#include "level.h"
#include "audio.h"
using namespace std;

int main(int argc, char* argv[])
{

    Uint32 lastTick = SDL_GetTicks();
    int currentLevel = 0;
    int remainingTime = gameLevels[currentLevel].timelimit;
    int score = 0;

    Graphics graphics;
    graphics.init();

    Pikachu pikachu;
    pikachu.init(gameLevels[currentLevel].imgcount);
    graphics.loadAllTextures();

    TTF_Font* font = graphics.loadFont("assets/dlxfont_.ttf", 28);

    std::vector<Button*> buttons;
    std::vector<Goiy*> goiy;
    graphics.initButton(graphics.renderer, buttons, goiy);


    bool isPause = false, isMuted = false;

    bool running = true;
    SDL_Event e;

    while (running)
    {
        Uint32 currentTick = SDL_GetTicks();
        if (!isPause && currentTick - lastTick >= 1000) {
            remainingTime--;
            lastTick = currentTick;
        }
        if (remainingTime <= 0) {
            // Reset về level 1
            currentLevel = 0;
            pikachu.init(gameLevels[currentLevel].imgcount);
            remainingTime = gameLevels[currentLevel].timelimit;
            isPause = false;
        }
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {

                running = false;
            }

            int x, y;
            SDL_GetMouseState(&x, &y);

            for (auto& btn : buttons) {
                if(btn->id != "Score" && btn->id != "Clock")
                    btn->updateHover(btn->isInside(x, y));
            }
            for (auto& g : goiy) {
                if (g->isInside(x, y)) {
                    g->updateHover(g->isInside(x,y));
                }
                else g->currentRect = g->baseRect;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                for (auto& btn : buttons) {
                    if (btn->isInside(x, y)) {
                        if (btn->id == "Pause") {
                            isPause = !isPause;
                        }
                        else if (btn->id == "Exit") {
                            running = false;
                        }
                        else if(btn->id == "Volume1"){
                            isMuted = !isMuted;

                        }
                        else if (btn->id == "New") {
                            currentLevel = 0;
                            score = 0;
                            pikachu.init(gameLevels[currentLevel].imgcount);
                            remainingTime = gameLevels[currentLevel].timelimit;
                            for (auto& g : goiy) {
                                g->click = 6;
                                g->texture = g->textures[5];
                                g->currentRect = g->baseRect;
                            }
                            isPause = false;
                        }
                    }
                }
                for (auto& g : goiy) {
                    if (g->isInside(x, y)) {
                        //  Gợi ý đường đi
                        pikachu.click_Goiy();
                        graphics.clicktoGoiy(graphics.renderer, pikachu);

                        g->click--;
                        if (g->click > 0) {
                            g->texture = g->textures[g->click - 1];
                        } else {
                            g->texture = nullptr;
                        }
                    }
                }
                if(!isPause)

                    graphics.handleMouseClick(x, y, pikachu, score, isMuted);



            }
        }
        if(!pikachu.isclear()){
            if (!pikachu.hasmove()) {
                pikachu.xaotron();
            }
        }else{
            graphics.audio.play(graphics.audio.win);
            pikachu.handleWin(currentLevel, remainingTime, gameLevels, goiy);
            SDL_Delay(200);
        }
        // Vẽ và cập nhật màn hình
        SDL_RenderClear(graphics.renderer);
        graphics.prepareScene(graphics.background);
        for (auto& btn : buttons){
            if(!isPause){
                if(!isMuted){
                    if(btn->id != "Play" && btn->id != "Volume2")
                        btn->render(graphics.renderer);
                }else{
                    if(btn->id != "Play" && btn->id != "Volume1")
                        btn->render(graphics.renderer);
                }

            }
            else {
                if(!isMuted){
                    if(btn->id != "Pause" && btn->id != "Volume2")
                        btn->render(graphics.renderer);
                }else{
                    if(btn->id != "Pause" && btn->id != "Volume1")
                        btn->render(graphics.renderer);
                }
            }
        }

        for (auto& g : goiy) {
            if (g->shouldRender()) {
                g->render(graphics.renderer);
            }
        }
        graphics.drawMap(pikachu.mp/*, pikachu.rows*/, pikachu, isPause);
        graphics.drawTime(graphics.renderer, font, remainingTime);
        graphics.drawScore(graphics.renderer, font, score);
        graphics.presentScene();
        SDL_Delay(100);
    }


    graphics.quit();
    SDL_Quit();
    return 0;
}
