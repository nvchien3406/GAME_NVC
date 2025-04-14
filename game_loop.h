#ifndef GAMELOOP_H_INCLUDED
#define GAMELOOP_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "graphics.h"
#include "logic.h"
#include "button.h"
#include "audio.h"
#include "level.h"
enum GameState{
    MENU,
    GAME,
    LOSE
};
std::vector<Button*> menu_button;
std::vector<Button*> lose_button;

//Menu
void renderMenuScreen(Graphics &graphics, TTF_Font* font, GameState& gamestate) {
    // Ve nen menu
    int x, y;
    SDL_GetMouseState(&x, &y);
    graphics.prepareScene(graphics.background_menu);

    // StartGame
    SDL_Color textColor = {255, 255, 255}; // Màu trắng
    SDL_Texture* startTexture = graphics.renderText("Start Game", font, textColor);
    int tw, th;
    SDL_QueryTexture(startTexture, NULL, NULL, &tw, &th);
    SDL_Rect startRect = {SCREEN_WIDTH / 2 - tw / 2, SCREEN_HEIGHT / 2 - th / 2 - 50, tw, th};
    Button* startButton = new Button(startTexture, startRect, "Start Game");
    menu_button.push_back(startButton);
    startButton->updateHover(startButton->isInside(x, y));  // hover
    startButton->render(graphics.renderer);
    SDL_DestroyTexture(startTexture);

    // Exit
    SDL_Texture* exitTexture = graphics.renderText("Exit", font, textColor);
    SDL_QueryTexture(exitTexture, NULL, NULL, &tw, &th);
    SDL_Rect exitRect = {SCREEN_WIDTH / 2 - tw / 2, SCREEN_HEIGHT / 2 - th / 2 + 50, tw, th};
    Button* exitButton = new Button(exitTexture, exitRect, "Exit");
    menu_button.push_back(exitButton);
    exitButton->updateHover(exitButton->isInside(x, y));  //hover
    exitButton->render(graphics.renderer);
    SDL_RenderCopy(graphics.renderer, exitTexture, NULL, &exitRect);
    SDL_DestroyTexture(exitTexture);

    graphics.presentScene();
}
void handleMenuEvents(SDL_Event &e, GameState &gamestate, bool &running, TTF_Font* font, Audio audio) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    for (auto& btn : menu_button) {
        btn->updateHover(btn->isInside(x, y));  // hover update
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {

        if (x >= SCREEN_WIDTH / 2 - 240 / 2 && x <= SCREEN_WIDTH / 2 + 240 / 2 && y >= SCREEN_HEIGHT / 2 - 25 / 2 - 50 && y <= SCREEN_HEIGHT / 2 - 25 / 2 - 20) {
            audio.play(audio.win);
            gamestate = GAME;
        }
        if (x >= SCREEN_WIDTH / 2 - 120 / 2 && SCREEN_WIDTH / 2 + 120 / 2 && y >= SCREEN_HEIGHT / 2 - 25 / 2 + 50 && y <= SCREEN_HEIGHT / 2 - 25 / 2 + 80) {
            running = false;
        }
    }
}
//Lose
void renderLoseScreen(Graphics &graphics, TTF_Font* font, GameState& gamestate) {
    // menu
    int x, y;
    SDL_GetMouseState(&x, &y);
    graphics.prepareScene(graphics.background_lose);

    // New Game
    SDL_Color textColor = {255, 255, 255}; // Màu trắng
    SDL_Texture* startTexture = graphics.renderText("New Game", font, textColor);
    int tw, th;
    SDL_QueryTexture(startTexture, NULL, NULL, &tw, &th);
    SDL_Rect startRect = {SCREEN_WIDTH / 2 - tw / 2, SCREEN_HEIGHT / 2 - th / 2 - 50, tw, th};
    Button* startButton = new Button(startTexture, startRect, "New Game");
    lose_button.push_back(startButton);
    startButton->updateHover(startButton->isInside(x, y));  //hover
    startButton->render(graphics.renderer);
    SDL_DestroyTexture(startTexture);

    //Exit
    SDL_Texture* exitTexture = graphics.renderText("Exit", font, textColor);
    SDL_QueryTexture(exitTexture, NULL, NULL, &tw, &th);
    SDL_Rect exitRect = {SCREEN_WIDTH / 2 - tw / 2, SCREEN_HEIGHT / 2 - th / 2 + 50, tw, th};
    Button* exitButton = new Button(exitTexture, exitRect, "Exit");
    lose_button.push_back(exitButton);
    exitButton->updateHover(exitButton->isInside(x, y));  //hover
    exitButton->render(graphics.renderer);
    SDL_RenderCopy(graphics.renderer, exitTexture, NULL, &exitRect);
    SDL_DestroyTexture(exitTexture);

    graphics.presentScene();
}
void handleLoseEvents(SDL_Event &e, GameState &gamestate, bool &running, TTF_Font* font, Audio audio, int &click) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    for (auto& btn : lose_button) {
        btn->updateHover(btn->isInside(x, y));
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {

        if (x >= SCREEN_WIDTH / 2 - 240 / 2 && x <= SCREEN_WIDTH / 2 + 240 / 2 && y >= SCREEN_HEIGHT / 2 - 25 / 2 - 50 && y <= SCREEN_HEIGHT / 2 - 25 / 2 - 20) {
            audio.play(audio.win);
            gamestate = GAME;
            click = 6;
        }
        if (x >= SCREEN_WIDTH / 2 - 120 / 2 && SCREEN_WIDTH / 2 + 120 / 2 && y >= SCREEN_HEIGHT / 2 - 25 / 2 + 50 && y <= SCREEN_HEIGHT / 2 - 25 / 2 + 80) {
            running = false;
        }
    }
}

void Timeupdate(Uint32 &lastTick, int &remainingTime, bool &isPause, int &currentLevel, Pikachu &pikachu, bool isPracticeMode, bool &isFlashing, GameState& game){
    if (isPracticeMode) return;
    Uint32 currentTick = SDL_GetTicks();
    if (!isPause && currentTick - lastTick >= 1000) {
        remainingTime--;
        lastTick = currentTick;
    }
    if (remainingTime <= 30) {
        isFlashing = !isFlashing;
    } else {
        isFlashing = false;
    }
    if (remainingTime <= 0) {
        //reset
        currentLevel = 0;
        pikachu.init(gameLevels[currentLevel].imgcount);
        remainingTime = gameLevels[currentLevel].timelimit;
        isPause = false;
        game = LOSE;
    }
}

void handleEvents(SDL_Event &e, std::vector<Button*> &buttons, Graphics &graphics,Pikachu &pikachu, int &score, bool &isMuted, bool &isPause, int &currentLevel, int &remainingTime, bool &isPracticeMode, int &clickGoiy, Audio audio, int &stoplevel) {
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
                if(!isMuted)
                    audio.play(audio.mouse);
                if (btn->id == "Pause") {
                    isPause = !isPause;
                }
                if (btn->id == "Exit") {
                    // Thoát game
                    SDL_Quit();
                }
                if(btn->id == "Volume1") {
                    isMuted = !isMuted;
                }
                if (btn->id == "New") {
                    // Đặt lại game
                    audio.play(audio.win);
                    currentLevel = 0;
                    score = 0;
                    pikachu.init(gameLevels[currentLevel].imgcount);
                    remainingTime = gameLevels[currentLevel].timelimit;
                    clickGoiy = 6;

                    isPause = false;
                    isPracticeMode = false;
                }
                if (btn->id == "Luyen") {
                    if(!isPracticeMode){
                        stoplevel = currentLevel;
                        isPracticeMode = !isPracticeMode;
                        isPause = false;
                        currentLevel = 0;
                        score = 0;
                        pikachu.init(gameLevels[currentLevel].imgcount);
                        remainingTime = gameLevels[currentLevel].timelimit;
                        clickGoiy = 6;
                    }else{
                        isPracticeMode = !isPracticeMode;
                        isPause = false;
                        currentLevel = stoplevel;
                        if(stoplevel == 0) score = 0;
                        else score = gameLevels[currentLevel - 1].maxscore;
                        pikachu.init(gameLevels[currentLevel].imgcount);
                        remainingTime = gameLevels[currentLevel].timelimit;
                        clickGoiy = 6;
                    }

                }
                if(btn->id == "Goiy"){
                    if(btn->texture && clickGoiy > 0){
                        --clickGoiy;
                        pikachu.click_Goiy();
                        graphics.clicktoGoiy(graphics.renderer, pikachu);
                    }
                    if(clickGoiy <= 0) btn->texture = nullptr;
                }
            }
        }

        if (!isPause && x > 280 && y > 175)
            graphics.handleMouseClick(x, y, pikachu, score, isMuted, audio);
    }
    if(clickGoiy > 0){
        for(auto& btn : buttons){
            if(btn->id == "Goiy") btn->texture = IMG_LoadTexture(graphics.renderer, "assets/image/button/goiy1.png");
        }
    }

}
void handleWin(Graphics &graphics, Pikachu & pikachu, int &currentLevel, int &remainingTime, int &clickGoiy, Audio audio, GameState& gamestate) {
    audio.play(audio.win);
    currentLevel++;
    if (currentLevel >= gameLevels.size()) {
        gamestate = LOSE;
        currentLevel = 0;
    }
    pikachu.init(gameLevels[currentLevel].imgcount);
    remainingTime = gameLevels[currentLevel].timelimit;

    clickGoiy = 6;

}
void renderScene(Graphics &graphics, Pikachu &pikachu, int &score, int &remainingTime, TTF_Font* font, bool isPause, bool isMuted, bool isPracticeMode, std::vector<Button*> &buttons, int &clickGoiy, int currentLevel, bool isFlashing) {
    SDL_RenderClear(graphics.renderer);
    graphics.prepareScene(graphics.background_game);
    // Vẽ các nút
    for (auto& btn : buttons) {
        if (btn->shouldRender()) {
            if (!isPause) {
                if (!isMuted && !isPracticeMode) {
                    if (btn->id != "Play" && btn->id != "Volume2" && btn->id != "ELuyen")
                        btn->render(graphics.renderer);
                }
                else if (!isMuted && isPracticeMode) {
                    if (btn->id != "Play" && btn->id != "Volume2" && btn->id != "Luyen")
                        btn->render(graphics.renderer);
                }
                else if (isMuted && !isPracticeMode) {
                    if (btn->id != "Play" && btn->id != "Volume1" && btn->id != "ELuyen")
                        btn->render(graphics.renderer);
                }
                else if (isMuted && isPracticeMode) {
                    if (btn->id != "Play" && btn->id != "Volume1" && btn->id != "Luyen")
                        btn->render(graphics.renderer);
                }
            } else {
                if (!isMuted && !isPracticeMode) {
                    if (btn->id != "Pause" && btn->id != "Volume2" && btn->id != "ELuyen")
                        btn->render(graphics.renderer);
                }
                else if (!isMuted && isPracticeMode) {
                    if (btn->id != "Pause" && btn->id != "Volume2" && btn->id != "Luyen")
                        btn->render(graphics.renderer);
                }
                else if (isMuted && !isPracticeMode) {
                    if (btn->id != "Pause" && btn->id != "Volume1" && btn->id != "ELuyen")
                        btn->render(graphics.renderer);
                }
                else if (isMuted && isPracticeMode) {
                    if (btn->id != "Pause" && btn->id != "Volume1" && btn->id != "Luyen")
                        btn->render(graphics.renderer);
                }
            }
        }
    }
    // Vẽ bản đồ game
    graphics.drawMap(pikachu.mp, pikachu, isPause);
    if(!isPracticeMode){
        graphics.drawTime(graphics.renderer, font, remainingTime);
        graphics.drawTimeBar(graphics.renderer, remainingTime, gameLevels[currentLevel].timelimit, isFlashing );
    }
    graphics.drawScore(graphics.renderer, font, score);
    graphics.drawLevel(graphics.renderer, font, currentLevel + 1);
    if(clickGoiy > 0)
        graphics.Goiy(graphics.renderer, font, clickGoiy);

    graphics.presentScene();
}

#endif
