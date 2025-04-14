#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include "level.h"
#include "defs.h"
#include "button.h"
#include <cstdlib>
#include <ctime>
#include<algorithm>
#include <unordered_map>
#include <vector>
#include <map>

struct Pikachu{
    int mp[9][16];// ban do
    int rows, cols;

    int selectedX = -1;
    int selectedY = -1;

    std::pair<int, int> hintA = {-1, -1};
    std::pair<int, int> hintB = {-1, -1};
    std::vector<std::pair<int, int>> hintPath;

    void init(int imgcount){
        rows = 9;
        cols = 16;
        taoMap(imgcount);
    }
    void taoMap(int numValues){
        srand(time(0));
        int totalTiles = rows * cols;

        std::vector<int> tileVal;
        std::vector<int> count(numValues + 1, 0);
        int sum = 0;

        // Duyệt từng số từ 1 -> numvalues và thêm vào tileVal với số lần chẵn ngẫu nhiên
        for (int i = 1; i <= numValues && sum < totalTiles; i++) {
            int add = 2 * ((rand() % 3) + 1); // Chọn số lần xuất hiện là chẵn (2, 4, 6)

            if (sum + add > totalTiles) {
                add = totalTiles - sum; // Nếu vượt quá tổng ô, chỉ thêm phần còn thiếu
            }

            if (add > 0) {
                count[i] = add;
                sum += add;
            }
        }

        // Nếu còn thiếu ô, tiếp tục điền số ngẫu nhiên với số lần chẵn
        while (sum < totalTiles) {
            int num = (rand() % numValues) + 1;
            if (sum + 2 <= totalTiles) {
                count[num] += 2;
                sum += 2;
            }
        }

        // Đưa các số vào mảng tileVal
        for (int i = 1; i <= numValues; i++) {
            for (int j = 0; j < count[i]; j++) {
                tileVal.push_back(i);
            }
        }

        // Trộn ngẫu nhiên
        std::random_shuffle(tileVal.begin(), tileVal.end());

        // Đưa vào bản đồ game Pikachu
        int pos = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                mp[i][j] = tileVal[pos++];
            }
        }
    }

    // CHECK COT VA HANG
    bool clearRow(int y1, int y2, int x){
        int min1 = std::min(y1, y2);
        int max1 = std::max(y1, y2);
        for(int i = min1 + 1; i < max1; i++){
            if(mp[x][i] != 0) return false;
        }
        return true;
    }

    bool clearCol(int x1, int x2, int y){
        int min1 = std::min(x1, x2);
        int max1 = std::max(x1, x2);
        for(int i = min1 + 1; i < max1; i++){
            if(mp[i][y] != 0) return false;
        }
        return true;
    }

    //Duong di hop le
    bool validmove(int x1, int y1, int x2, int y2){
        if(mp[x1][y1] != mp[x2][y2] || mp[x1][y1] == 0 || (x1 == x2 && y1 == y2)) return false;

        //Noi duong thang
        if(x1 == x2 && clearRow(y1, y2, x1)) return true;
        if(y1 == y2 && clearCol(x1, x2, y1)) return true;
        //1 diem ngoat
        if (clearRow(y1, y2, x1) && clearCol(x1, x2, y2) && mp[x1][y2] == 0) return true;
        if (clearCol(x1, x2, y1) && clearRow(y1, y2, x2) && mp[x2][y1] == 0) return true;

        // 2 diem ngoat
        for (int i = 0; i < rows; i++) {
            if (mp[i][y1] == 0 && mp[i][y2] == 0
                && clearCol(x1, i, y1)
                && clearCol(x2, i, y2)
                && clearRow(y1, y2, i))
                return true;
        }
        for (int j = 0; j < cols; j++) {
            if (mp[x1][j] == 0 && mp[x2][j] == 0
                && clearRow(y1, j, x1)
                && clearRow(y2, j, x2)
                && clearCol(x1, x2, j))
                return true;
        }

        //Truong hop ra bien
        if((clearRow(y1, 16, x1) && clearRow(y2, 16, x2)) || (clearRow(y1, -1, x1) && clearRow(y2, -1, x2) )) return true;
        if((clearCol(-1, x1, y1) && clearCol(-1, x2, y2))|| (clearCol(9, x1, y1) && clearCol(9, x2, y2))) return true;
        return false;
    }

    //Kiem tra xem con duong di khong?
    bool hasmove() {
        if(isclear()){
            return false;
        }
        std::unordered_map<int, std::vector<std::pair<int, int>>> valuePositions;


        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (mp[i][j] != 0) {
                    valuePositions[mp[i][j]].push_back({i, j});
                }
            }
        }


        for (const auto& entry : valuePositions) {
            const std::vector<std::pair<int, int>>& positions = entry.second;
            for (size_t i = 0; i < positions.size(); i++) {
                for (size_t j = i + 1; j < positions.size(); j++) {
                    int x1 = positions[i].first, y1 = positions[i].second;
                    int x2 = positions[j].first, y2 = positions[j].second;


                    if (validmove(x1, y1, x2, y2)) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    //Delete cap hinh
    void removepair(int x1, int y1, int x2, int y2){
        mp[x1][y1] = 0;
        mp[x2][y2] = 0;
    }

    //Chuyen man
    bool isclear(){
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (mp[i][j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }


    //Neu khong con duong di thi xao tron
    void shuffle(){
        std::vector<std::pair<int, int>> pos; // Lưu vị trí các ô có hình
        std::vector<int> values; // Lưu giá trị của các ô có hình

        // Lưu tất cả ô có hình vào danh sách
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (mp[i][j] != 0) {
                    pos.push_back({i, j});
                    values.push_back(mp[i][j]);
                }
            }
        }

        // Xáo trộn cho đến khi có đường đi hợp lệ
        do {
            for (int i = values.size() - 1; i > 0; i--) {
                int j = rand() % (i + 1);
                std::swap(values[i], values[j]);
            }

            // Gán lại vào bản đồ
            for (size_t i = 0; i < pos.size(); i++) {
                mp[pos[i].first][pos[i].second] = values[i];
            }

        } while (!hasmove()); // Lặp lại nếu không có nước đi hợp lệ
    }

    std::vector<std::pair<int, int>> duongdi(int x1, int y1, int x2, int y2){
        int minx = std::min(x1, x2);
        int maxx = std::max(x1, x2);
        int miny = std::min(y1, y2);
        int maxy = std::max(y1, y2);
        if(!validmove(x1, y1, x2, y2)) return {};
        std::vector<std::pair<int, int>> v;
        //Noi duong thang
        if(x1 == x2 && clearRow(y1, y2, x1)){
            addLine(v, x1, y1, x2, y2);
            return v;
        }
        if(y1 == y2 && clearCol(x1, x2, y1)){
            addLine(v, x1, y1, x2, y2);
            return v;
        }
        //1 diem ngoat
        if (clearRow(y1, y2, x1) && clearCol(x1, x2, y2) && mp[x1][y2] == 0){
            addLine(v, x1, y1, x1, y2);
            addLine(v, x1, y2, x2, y2);
            return v;
        }
        if (clearCol(x1, x2, y1) && clearRow(y1, y2, x2) && mp[x2][y1] == 0){
            addLine(v, x1, y1, x2, y1);
            addLine(v, x2, y1, x2, y2);
            return v;
        }

        // 2 diem ngoat
        for (int i = 0; i < rows; i++) {
            if (mp[i][y1] == 0 && mp[i][y2] == 0 && clearCol(x1, i, y1) && clearCol(x2, i, y2) && clearRow(y1, y2, i)){
                addLine(v, x1, y1, i, y1);
                addLine(v, i, y1, i, y2);
                addLine(v, i, y2, x2, y2);
                return v;
            }

        }
        for (int i = 0; i < cols; i++) {
            if (mp[x1][i] == 0 && mp[x2][i] == 0 && clearRow(y1, i, x1) && clearRow(y2, i, x2) && clearCol(x1, x2, i)){
                addLine(v, x1, y1, x1, i);
                addLine(v, x1, i, x2, i);
                addLine(v, x2, i, x2, y2);
                return v;
            }
        }

        //Truong hop ra bien
        if((clearRow(y1, 16, x1) && clearRow(y2, 16, x2)) || (clearRow(y1, -1, x1) && clearRow(y2, -1, x2) )){
            if(clearRow(y1, 16, x1) && clearRow(y2, 16, x2)){
                addLine(v, x1, y1, x1, 16);
                addLine(v, x1, 16, x2, 16);
                addLine(v, x2, 16, x2, y2);
            }
            else{
                addLine(v, x1, y1, x1, -1);
                addLine(v, x1, -1, x2, -1);
                addLine(v, x2, -1, x2, y2);
            }
            return v;
        }
        if((clearCol(-1, x1, y1) && clearCol(-1, x2, y2))|| (clearCol(9, x1, y1) && clearCol(9, x2, y2))){
            if(clearCol(-1, x1, y1) && clearCol(-1, x2, y2)){
                addLine(v, x1, y1, -1, y1);
                addLine(v, -1, y1, -1, y2);
                addLine(v, -1, y2, x2, y2);
            }else{
                addLine(v, x1, y1, 9, y1);
                addLine(v, 9, y1, 9, y2);
                addLine(v, 9, y2, x2, y2);
            }
            return v;
        }
        return {};
    }

    void click_Goiy(){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                if(mp[i][j] == 0) continue;
                for(int k = 0; k < rows; k++){
                    for(int m = 0; m < cols; m++){
                        if(validmove(i, j, k, m)){
                            hintA = {i, j};
                            hintB = {k, m};
                            hintPath = duongdi(i, j, k, m);
                            return;
                        }
                    }
                }
            }
        }
    }
    void addLine(std::vector<std::pair<int, int>> &v, int xStart, int yStart, int xEnd, int yEnd){
        if(xStart == xEnd){
            int step = yStart < yEnd ? 1 : -1;
            for (int y = yStart; y != yEnd + step; y += step)
                v.push_back({xStart, y});
        }
        else if(yStart == yEnd){
            int step = xStart < xEnd ? 1 : -1;
            for(int x = xStart; x != xEnd + step; x += step)
                v.push_back({x, yStart});
        }
    }
};

#endif // LOGIC_H_INCLUDED
