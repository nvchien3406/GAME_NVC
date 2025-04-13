#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <string>

struct Button
{
    SDL_Texture* texture;
    SDL_Rect baseRect;
    SDL_Rect currentRect;
    std::string id;
    bool check = false; //check hover
    Button(SDL_Texture* texture, SDL_Rect baseRect, std::string id) : texture(texture), baseRect(baseRect), id(id), currentRect(baseRect) {}
    void updateHover(bool hover) {
        check = hover;
        if (hover) {
            int dw = baseRect.w * 0.1;
            int dh = baseRect.h * 0.1;
            currentRect.x = baseRect.x - dw / 2;
            currentRect.y = baseRect.y - dh / 2;
            currentRect.w = baseRect.w + dw;
            currentRect.h = baseRect.h + dh;
        } else {
            currentRect = baseRect;
        }
    }
    bool isInside(int x, int y) {
        return x >= currentRect.x && x <= currentRect.x + currentRect.w &&
               y >= currentRect.y && y <= currentRect.y + currentRect.h;
    }
    //ve nut len
    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, nullptr, &currentRect);
    }
    bool shouldRender() const {
        return texture != nullptr;
    }

};
#endif // BUTTON_H_INCLUDED
