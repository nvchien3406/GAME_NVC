#include <iostream>
#include <SDL.H>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "graphics.h"
#include "logic.h"

using namespace std;

int main(int argc, char* argv[]) {
    Pikachu game;
    game.init(6); // Bắt đầu với 6x6

    // In ra bản đồ
    for (int i = 0; i < game.rows; i++) {
        for (int j = 0; j < game.cols; j++) {
            std::cout << game.map[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
