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

void handleEvents(SDL_Event &e, std::vector<Button*> &buttons, Graphics &graphics,Pikachu &pikachu, int &score, bool &isMuted, bool &isPause, int &currentLevel, int &remainingTime, bool &isPracticeMode, int &clickGoiy) {
    int x, y;
    SDL_GetMouseState(&x, &y);

    // cap nhat hover cac nut
    for (auto& btn : buttons) {
        if(btn->id != "Score" && btn->id != "Clock")
            btn->updateHover(btn->isInside(x, y));
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
                    clickGoiy = 6;

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
                    clickGoiy = 6;

                }
                else if(btn->id == "Goiy"){
                    if(btn->texture && clickGoiy > 0){
                        --clickGoiy;
                        pikachu.click_Goiy();
                        graphics.clicktoGoiy(graphics.renderer, pikachu);
                    }
                    if(clickGoiy <= 0) btn->texture = nullptr;
                }
            }
        }
        // Nếu không pause, xử lý click trên game
        if (!isPause)
            graphics.handleMouseClick(x, y, pikachu, score, isMuted);
    }

}
void handleWin(Graphics &graphics, Pikachu & pikachu, int &currentLevel, int &remainingTime, int &clickGoiy) {
    graphics.audio.play(graphics.audio.win);
    currentLevel++;
    if (currentLevel >= gameLevels.size()) {
        currentLevel = 0; // Quay lại level 1
    }
    pikachu.init(gameLevels[currentLevel].imgcount);
    remainingTime = gameLevels[currentLevel].timelimit;

    // Đặt lại trạng thái gợi ý

    clickGoiy = 6;

}
void renderScene(Graphics &graphics, Pikachu &pikachu, int &score, int &remainingTime, TTF_Font* font, bool isPause, bool isMuted, bool isPracticeMode, std::vector<Button*> &buttons, int &clickGoiy, int currentLevel) {
    SDL_RenderClear(graphics.renderer);
    graphics.prepareScene(graphics.background);

    // Vẽ các nút điều khiển
    for (auto& btn : buttons) {
        if(btn->shouldRender()){
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
    }



    // Vẽ bản đồ game
    graphics.drawMap(pikachu.mp, pikachu, isPause);
    if(!isPracticeMode) graphics.drawTime(graphics.renderer, font, remainingTime);
    graphics.drawScore(graphics.renderer, font, score);
    //graphics.drawScore1(graphics.renderer, font1);
    graphics.drawLevel(graphics.renderer, font, currentLevel + 1);
    if(clickGoiy > 0)
        graphics.drawLevel1(graphics.renderer, font, clickGoiy);

    graphics.presentScene();
}

#endif
