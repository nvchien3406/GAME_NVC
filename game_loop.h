#ifndef GAMELOOP_H_INCLUDED
#define GAMELOOP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "graphics.h"
#include "logic.h"
#include "button.h"
#include "audio.h"
#include "level.h"

void Timeupdate(Uint32 &lastTick, int &remainingTime, bool &isPause, int &currentLevel, Pikachu &pikachu, bool isPracticeMode){
    if (isPracticeMode) return;
    Uint32 currentTick = SDL_GetTicks();
    if (!isPause && currentTick - lastTick >= 1000) {
        remainingTime--;
        lastTick = currentTick;
    }
    if (remainingTime <= 0) {
        // Reset về level 1 khi hết thời gian
        currentLevel = 0;
        pikachu.init(gameLevels[currentLevel].imgcount);
        remainingTime = gameLevels[currentLevel].timelimit;
        isPause = false;
    }
}

void handleEvents(SDL_Event &e, std::vector<Button*> &buttons, Goiy* &goiy, Graphics &graphics,Pikachu &pikachu, int &score, bool &isMuted, bool &isPause, int &currentLevel, int &remainingTime, bool &isPracticeMode) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    // cap nhat hover cac nut
    for (auto& btn : buttons) {
        if(btn->id != "Score" && btn->id != "Clock")
            btn->updateHover(btn->isInside(x, y));
    }

    // cap nhat goi y
    if (goiy->isInside(x, y)) {
        goiy->updateHover(goiy->isInside(x,y));
    } else {
        goiy->currentRect = goiy->baseRect;
    }


    if (e.type == SDL_MOUSEBUTTONDOWN) {
        for (auto& btn : buttons) {
            if (btn->isInside(x, y)) {
                if (btn->id == "Pause") {
                    isPause = !isPause;
                }
                else if (btn->id == "Exit") {
                    // Thoát game
                    SDL_Quit();
                }
                else if(btn->id == "Volume1") {
                    isMuted = !isMuted;
                }
                else if (btn->id == "New") {
                    // Đặt lại game
                    currentLevel = 0;
                    score = 0;
                    pikachu.init(gameLevels[currentLevel].imgcount);
                    remainingTime = gameLevels[currentLevel].timelimit;
                    goiy->click = 6;
                    goiy->currentRect = goiy->baseRect;

                    isPause = false;
                    isPracticeMode = false;
                }
                else if (btn->id == "Luyen") {
                    isPracticeMode = true;
                    isPause = false;
                    currentLevel = 0;
                    score = 0;
                    pikachu.init(gameLevels[currentLevel].imgcount);
                    // Không set lại thời gian
                    goiy->click = 6;
                    goiy->currentRect = goiy->baseRect;

                }
            }
        }

        // Xử lý click gợi ý
        if (goiy->isInside(x, y)) {
            if(goiy->texture){
                pikachu.click_Goiy();
                graphics.clicktoGoiy(graphics.renderer, pikachu);
            }


            goiy->click--;
            if(goiy->click <= 0) {
                goiy->texture = nullptr;
            }
        }


        // Nếu không pause, xử lý click trên game
        if (!isPause)
            graphics.handleMouseClick(x, y, pikachu, score, isMuted);
    }

}
void handleWin(Graphics &graphics, Pikachu & pikachu, int &currentLevel, int &remainingTime, Goiy* &goiy) {
    graphics.audio.play(graphics.audio.win);
    currentLevel++;
    if (currentLevel >= gameLevels.size()) {
        currentLevel = 0; // Quay lại level 1
    }
    pikachu.init(gameLevels[currentLevel].imgcount);
    remainingTime = gameLevels[currentLevel].timelimit;

    // Đặt lại trạng thái gợi ý

    goiy->click = 6;
    goiy->currentRect = goiy->baseRect;

}
void renderScene(Graphics &graphics, Pikachu &pikachu, int &score, int &remainingTime, TTF_Font* font, bool isPause, bool isMuted, bool isPracticeMode, std::vector<Button*> &buttons, Goiy* &goiy, int currentLevel) {
    SDL_RenderClear(graphics.renderer);
    graphics.prepareScene(graphics.background);

    // Vẽ các nút điều khiển
    for (auto& btn : buttons) {
        if (!isPause) {
            if (!isMuted) {
                if (btn->id != "Play" && btn->id != "Volume2")
                    btn->render(graphics.renderer);
            } else {
                if (btn->id != "Play" && btn->id != "Volume1")
                    btn->render(graphics.renderer);
            }
        } else {
            if (!isMuted) {
                if (btn->id != "Pause" && btn->id != "Volume2")
                    btn->render(graphics.renderer);
            } else {
                if (btn->id != "Pause" && btn->id != "Volume1")
                    btn->render(graphics.renderer);
            }
        }
    }

    // Vẽ các gợi ý
    if (goiy->shouldRender()) {
        goiy->render(graphics.renderer);
    }


    // Vẽ bản đồ game
    graphics.drawMap(pikachu.mp, pikachu, isPause);
    if(!isPracticeMode) graphics.drawTime(graphics.renderer, font, remainingTime);
    graphics.drawScore(graphics.renderer, font, 2160);
    //graphics.drawScore1(graphics.renderer, font1);
    graphics.drawLevel(graphics.renderer, font, currentLevel + 1);
    if(goiy->click > 0)
        graphics.drawLevel1(graphics.renderer, font, goiy->click);

    graphics.presentScene();
}

#endif
