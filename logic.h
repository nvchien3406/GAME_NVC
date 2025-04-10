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
        //int numValues = 36; //so con vat tu 1 -> 36

        std::vector<int> tileVal;
        std::vector<int> count(numValues + 1, 0);
        int sum = 0;

        // Duyệt từng số từ 1 -> numvalues và thêm vào tileVal với số lần chẵn ngẫu nhiên
        for (int i = 1; i <= numValues && sum < totalTiles; i++) {
            int add = 2 * ((rand() % 3) + 1); // Chọn số lần xuất hiện là chẵn (2, 4, 6)

            if (sum + add > totalTiles) {
                add = totalTiles - sum; // Nếu vượt quá tổng ô, chỉ thêm phần còn thiếu
                if (add % 2 == 1) add--; // Đảm bảo chẵn
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
    void xaotron(){
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
        std :: map<int, int> point;
        point[x1] = y1;
        point[x2] = y2;
        point[y1] = x1;
        point[y2] = x2;
        int minx = std::min(x1, x2);
        int maxx = std::max(x1, x2);
        int miny = std::min(y1, y2);
        int maxy = std::max(y1, y2);
        if(!validmove(x1, y1, x2, y2)) return {};
        std::vector<std::pair<int, int>> v;
        //Noi duong thang
        if(x1 == x2 && clearRow(y1, y2, x1)){
            for(int i = miny; i <= maxy; i++){
                v.push_back({x1, i});
            }
            return v;
        }
        if(y1 == y2 && clearCol(x1, x2, y1)){
            for(int i = minx; i <= maxx; i++){
                v.push_back({i, y1});

            }
            return v;
        }
        //1 diem ngoat
        if (clearRow(y1, y2, x1) && clearCol(x1, x2, y2) && mp[x1][y2] == 0){
            if(y1 > y2){
                if(x1 > x2){
                    for(int i = x2; i <= x1; i++){
                        v.push_back({i, y2});
                    }
                    for(int i = y2 + 1; i <= y1; i++){
                        v.push_back({x1, i});
                    }

                }else{
                    for(int i = y1; i >= y2; i--){
                        v.push_back({x1, i});
                    }
                    for(int i = x1; i <= x2; i++){
                        v.push_back({i, y2});
                    }
                }
            }else{
                if(x1 > x2){
                    for(int i = y1; i <= y2; i++){
                        v.push_back({x1, i});
                    }
                    for(int i = x1; i >= x2; i--){
                        v.push_back({i, y2});
                    }

                }else{
                    for(int i = y1; i <= y2; i++){
                        v.push_back({x1, i});
                    }
                    for(int i = x1; i <= x2; i++){
                        v.push_back({i, y2});
                    }
                }
            }

            return v;
        }
        if (clearCol(x1, x2, y1) && clearRow(y1, y2, x2) && mp[x2][y1] == 0){

            if(y1 > y2){
                if(x1 > x2){
                    for(int i = y2; i <= y1; i++){
                        v.push_back({x2, i});
                    }
                    for(int i = x2; i <= x1; i++){
                        v.push_back({i, y1});
                    }

                }else{
                    for(int i = y2; i <= y1; i++){
                        v.push_back({x2, i});
                    }
                    for(int i = x2; i >= x1; i--){
                        v.push_back({i, y1});
                    }
                }
            }else{
                if(x1 > x2){
                    for(int i = x1; i >= x2; i--){
                        v.push_back({i, y1});
                    }
                    for(int i = y1; i <= y2; i++){
                        v.push_back({x2, i});
                    }

                }else{
                    for(int i = x1; i <= x2; i++){
                        v.push_back({i, y1});
                    }
                    for(int i = y1; i <= y2; i++){
                        v.push_back({x2, i});
                    }
                }
            }

            return v;
        }

        // 2 diem ngoat
        for (int i = 0; i < rows; i++) {
            if (mp[i][y1] == 0 && mp[i][y2] == 0 && clearCol(x1, i, y1) && clearCol(x2, i, y2) && clearRow(y1, y2, i)){
                if(i < minx){
                    for(int j = point[miny]; j >= i; j--){
                        v.push_back({j, miny});
                    }
                    for(int j = miny; j<= maxy; j++){
                        v.push_back({i, j});
                    }
                    for(int j = i; j <= point[maxy]; j++ ){
                        v.push_back({j, maxy});
                    }
                }
                else if(i > minx && i < maxx){
                    if(point[maxx] < point[minx]){
                        for(int j = maxx; j >= i; j--){
                            v.push_back({j, point[maxx]});
                        }
                        for(int j = miny; j <= maxy; j++){
                            v.push_back({i, j});
                        }
                        for(int j = i; j >= minx; j--){
                            v.push_back({j, point[minx]});
                        }
                    }
                    else{
                        for(int j = minx; j<= i; j++){
                            v.push_back({j, point[minx]});
                        }
                        for(int j = miny; j <= maxy; j++){
                            v.push_back({i, j});
                        }
                        for(int j = i; j<= maxx; j++){
                            v.push_back({j, point[maxx]});
                        }
                    }
                }
                else{
                    for(int j = point[miny]; j <= i; j++){
                        v.push_back({j, miny});
                    }
                    for(int j = miny; j<= maxy; j++){
                        v.push_back({i, j});
                    }
                    for(int j = i; j >= point[maxy]; j-- ){
                        v.push_back({j, maxy});
                    }
                }
                return v;
            }

        }
        for (int i = 0; i < cols; i++) {
            if (mp[x1][i] == 0 && mp[x2][i] == 0 && clearRow(y1, i, x1) && clearRow(y2, i, x2) && clearCol(x1, x2, i)){
                if(i < miny){
                    for(int j = point[minx]; j >= i; j--){
                        v.push_back({minx, j});
                    }
                    for(int j = minx; j<= maxx; j++){
                        v.push_back({j, i});
                    }
                    for(int j = i; j <= point[maxx]; j++ ){
                        v.push_back({maxx, j});
                    }
                }
                else if(i > miny && i < maxy){
                    if(point[maxy] < point[miny]){
                        for(int j = maxy; j >= i; j--){
                            v.push_back({minx, j});
                        }
                        for(int j = minx; j <= maxx; j++){
                            v.push_back({j, i});
                        }
                        for(int j = i; j >= miny; j--){
                            v.push_back({maxx, j});
                        }
                    }
                    else{
                        for(int j = miny; j<= i; j++){
                            v.push_back({minx, j});
                        }
                        for(int j = minx; j <= maxx; j++){
                            v.push_back({j, i});
                        }
                        for(int j = i; j<= maxy; j++){
                            v.push_back({maxx, j});
                        }
                    }
                }
                else{
                    for(int j = point[minx]; j <= i; j++){
                        v.push_back({minx, j});
                    }
                    for(int j = minx; j<= maxx; j++){
                        v.push_back({j, i});
                    }
                    for(int j = i; j >= point[maxx]; j-- ){
                        v.push_back({maxx, j});
                    }
                }
                return v;
            }
        }

        //Truong hop ra bien
        if((clearRow(y1, 16, x1) && clearRow(y2, 16, x2)) || (clearRow(y1, -1, x1) && clearRow(y2, -1, x2) )){
            if(clearRow(y1, 16, x1) && clearRow(y2, 16, x2)){
                if(x1 < x2){
                    for(int i = y1; i <= 16; i++){
                        v.push_back({x1, i});
                    }
                    for(int i = x1; i <= x2; i++){
                        v.push_back({i,16});
                    }
                    for(int i = 16; i >= y2; i--){
                        v.push_back({x2, i});
                    }
                }else{
                    for(int i = y2; i <= 16; i++){
                        v.push_back({x2, i});
                    }
                    for(int i = x2;i<= x1; i++){
                        v.push_back({i, 16});
                    }
                    for(int i = 16; i >= y1; i--){
                        v.push_back({x1, i});
                    }
                }
            }
            else{
                if(x1 < x2){
                    for(int i = y1; i >= -1; i--){
                        v.push_back({x1, i});
                    }
                    for(int i = x1; i <= x2; i++){
                        v.push_back({i, -1});
                    }
                    for(int i = -1; i <= y2; i++){
                        v.push_back({x2, i});
                    }
                }
                else{
                    for(int i = y2; i >= -1; i--){
                        v.push_back({x2, i});
                    }
                    for(int i = x2; i <= x1; i++){
                        v.push_back({i, -1});
                    }
                    for(int i = -1; i <= y1; i++){
                        v.push_back({x1, i});
                    }
                }
            }
            return v;
        }
        if((clearCol(-1, x1, y1) && clearCol(-1, x2, y2))|| (clearCol(9, x1, y1) && clearCol(9, x2, y2))){
            if(clearCol(-1, x1, y1) && clearCol(-1, x2, y2)){
                if(y1 < y2){
                    for(int i = x1; i >= -1; i--){
                        v.push_back({i, y1});
                    }
                    for(int i = y1; i <= y2; i++){
                        v.push_back({-1, i});
                    }
                    for(int i = -1; i <= x2; i++){
                        v.push_back({i, y2});
                    }
                }
                else{
                    for(int i = x2; i >= -1; i--){
                        v.push_back({i, y2});
                    }
                    for(int i = y2; i <= y1; i++){
                        v.push_back({-1, i});
                    }
                    for(int i = -1; i <= x1; i++){
                        v.push_back({i, y1});
                    }
                }
            }else{
                if(y1 < y2){
                    for(int i = x1; i <= 9; i++){
                        v.push_back({i, y1});
                    }
                    for(int i = y1; i <= y2; i++){
                        v.push_back({9, i});
                    }
                    for(int i = 9; i >= x2; i--){
                        v.push_back({i, y2});
                    }
                }
                else{
                    for(int i = x2; i <= 9; i++){
                        v.push_back({i, y2});
                    }
                    for(int i = y2; i <= y1; i++){
                        v.push_back({9, i});
                    }
                    for(int i = 9; i >= x1; i--){
                        v.push_back({i, y1});
                    }
                }
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
    void handleWin(int& currentLevel, int& remainingTime, const std::vector<Level>& gameLevels,const std::vector<Goiy*>& goiy) {
        currentLevel++;
        if (currentLevel >= gameLevels.size()) {
            currentLevel = 0; // Quay lại level 1
        }

        init(gameLevels[currentLevel].imgcount); // Khởi tạo lại map
        remainingTime = gameLevels[currentLevel].timelimit;
        for(auto&g : goiy){
            g->click = 6;
            g->texture = g->textures[5];
            g->currentRect = g->baseRect;
        }
    }
};

#endif // LOGIC_H_INCLUDED
