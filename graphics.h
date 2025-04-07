#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"
#include "logic.h"

struct Graphics{
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Texture *textures[36];

    SDL_Texture *background = nullptr;

    void loadAllTextures()
    {
        for (int i = 0; i < 36; i++)
        {
            std::string filename = "image/" + std::to_string(i) + ".png";
            textures[i] = loadTexture(filename.c_str());

            if (textures[i] == nullptr)
            {
                SDL_Log("Failed to load %s", filename.c_str());
            }
        }
        background = IMG_LoadTexture(renderer, "image/background.jpg");
        if (!background) {
            std::cerr << "Failed to load background: " << SDL_GetError() << std::endl;
        }
    }


    void handleMouseClick(int x, int y, Pikachu& pikachu) {
        int gridSize = pikachu.rows;
        int tileSize1 = 40;
        int tileSize2 = 50;

        //Chuyển tọa độ thành ô
        int i = (y - 200) / tileSize2;
        int j = (x - 280) / tileSize1;


        if (i >= 0 && i < 9 && j >= 0 && j < 16) {
            int tile = pikachu.mp[i][j];
            if (tile != 0) {

                if (pikachu.selectedX == -1 && pikachu.selectedY == -1) {

                    pikachu.selectedX = i;
                    pikachu.selectedY = j;
                } else {

                    if (pikachu.validmove(pikachu.selectedX, pikachu.selectedY, i, j)) {
                        std::vector<std::pair<int, int>> path = pikachu.duongdi(pikachu.selectedX, pikachu.selectedY, i, j);
                        if(!path.empty()){
                            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                            for (size_t i = 0; i < path.size() - 1; i++) {
                                int x1 = 280 + path[i].second * tileSize1 + tileSize1 / 2;
                                int y1 = 200 + path[i].first * tileSize2 + tileSize2 / 2;
                                int x2 = 280 + path[i + 1].second * tileSize1 + tileSize1 / 2;
                                int y2 = 200 + path[i + 1].first * tileSize2 + tileSize2 / 2;
                                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                                SDL_RenderDrawLine(renderer, x1+1, y1+1, x2+1, y2+1);
                                SDL_RenderDrawLine(renderer, x1-1, y1-1, x2-1, y2-1);
                                SDL_RenderDrawLine(renderer, x1+1, y1-1, x2+1, y2-1);
                                SDL_RenderDrawLine(renderer, x1-1, y1+1, x2-1, y2+1);
                                SDL_RenderDrawLine(renderer, x1-1, y1-1, x2-1, y2-1);
                            }
                            SDL_RenderPresent(renderer);
                            SDL_Delay(300);
                        }
                        pikachu.removepair(pikachu.selectedX, pikachu.selectedY, i, j);

                        pikachu.selectedX = -1;
                        pikachu.selectedY = -1;
                    } else {

                        pikachu.selectedX = -1;
                        pikachu.selectedY = -1;
                    }
                }
            }
        }
    }

    void drawMap(int mp[9][16]/*, int gridSize*/, Pikachu &pikachu)
    {
        int tileSize1 = 40;
        int tileSize2 = 50;

        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 16; j++)
            {
                int type = mp[i][j];

                if (type > 0 && type < 37)
                {
                    int x = 280 + j * tileSize1;
                    int y = 200 + i * tileSize2;
                    renderTexture(textures[type - 1], x, y);

                    if (i == pikachu.selectedX && j == pikachu.selectedY) {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu do
                        SDL_Rect highlightRect1 = { x, y, tileSize1, tileSize2 };
                        SDL_Rect highlightRect2 = { x + 1, y + 1, tileSize1 - 2, tileSize2 - 2 };
                        SDL_RenderDrawRect(renderer, &highlightRect1);
                        SDL_RenderDrawRect(renderer, &highlightRect2);
                    }
                }
            }
        }
    }


    void logErrorAndExit(const char* msg, const char* error)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

    void init(){
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
            logErrorAndExit("SDL_Init", SDL_GetError());
        }

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

        //Tang chat luong hien thi trong SDL2
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());
        }

        //Initialize SDL_mixer
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            logErrorAndExit( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        }
    }

    void prepareScene()
    {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

    }

    void prepareScene(SDL_Texture * background)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy( renderer, background, nullptr, nullptr);
    }
    //hien thi hinh anh
    void presentScene()
    {
        SDL_RenderPresent(renderer);
    }
    //load hinh anh len renderer
    SDL_Texture *loadTexture(const char *filename)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    // ve texture len renderer
    void renderTexture(SDL_Texture *texture, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    //ve 1 phan texture
    void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
    {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    void quit()
    {
        Mix_Quit();
        TTF_Quit();
        IMG_Quit();
        for (int i = 0; i < 36; ++i) {
            if (textures[i]) {
                SDL_DestroyTexture(textures[i]);
                textures[i] = nullptr;
            }
        }
        if (background) {
            SDL_DestroyTexture(background);
            background = nullptr;
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    //ttf
    TTF_Font* loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
        }
        return gFont;
    }

    SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text, textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Create texture from text %s", SDL_GetError());
        }

        SDL_FreeSurface( textSurface );
        return texture;
    }

    //mixer

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
};

#endif // GRAPHICS_H
