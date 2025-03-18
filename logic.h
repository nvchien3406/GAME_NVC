#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include "defs.h"
#include <cstdlib>
#include <ctime>

struct Pikachu{
    int map[MAX_SIZE][MAX_SIZE];// ban do
    int rows, cols;


    void init(int size){
        rows = size;
        cols = size;
        taoMap();
    }
    void taoMap(){
        srand(time(0)); //random
        int totalTiles = rows * cols;
        int tilepair = totalTiles / 2;

        int tileVal[totalTiles];
        for(int i = 0; i < tilepair; i++){
            tileVal[i * 2] = i + 1;
            tileVal[i * 2  + 1] = i + 1;
        }

        //TRON NGAU NHIEN
        for(int i = totalTiles - 1; i >= 0; i--){
            int j = rand() % (i + 1);
            std :: swap(tileVal[i], tileVal[j]);
        }

        //CHO VAO MAP
        int pos = 0;
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                map[i][j] = (pos < totalTiles) ? tileVal[pos++] : 0;
            }
        }
    }

    //Duong di hop le
    bool validmove(int x1, int y1, int x2, int y2){
        return false;
    }

    //Kiem tra xem con duong di khong?
    bool hasmove(){
        return false;
    }

    //Delete cap hinh
    void removepair(int x1, int y1, int x2, int y2){
        map[x1][y1] = 0;
        map[x2][y2] = 0;
    }

    //Chuyen man
    void nextLevel(){
        if(rows < MAX_SIZE && cols < MAX_SIZE){
            init(rows + 2);
        }
    }
};

#endif // LOGIC_H_INCLUDED
