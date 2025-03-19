#include <iostream>
#include <SDL.h>  // Chỉnh sửa lại tên header
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "defs.h"
#include "graphics.h"
#include "logic.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Khởi tạo Graphics và Pikachu
    Graphics graphics;
    graphics.init();

    Pikachu pikachu;
    pikachu.init(6);  // Khởi tạo với kích thước ban đầu (6x6)
    graphics.loadAllTextures(); // Load tất cả các hình ảnh

    bool running = true;
    SDL_Event e;

    while (running)
    {

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                //break;
                running = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                graphics.handleMouseClick(x, y, pikachu);
            }
        }
        if(!pikachu.isclear()){
            if (!pikachu.hasmove()) {
                pikachu.xaotron();
            }
        }

        else{
            pikachu.nextLevel();
        }
        // Vẽ và cập nhật màn hình
        graphics.prepareScene();
        graphics.drawMap(pikachu.mp, pikachu.rows, pikachu);
        graphics.presentScene();

        SDL_Delay(100);
    }


    graphics.quit();
    SDL_Quit();
    cout << "Main function ending now.\n";
    return 0;
}
