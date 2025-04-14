#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include <vector>
struct Level{
    int imgcount;
    int timelimit;
    int maxscore;
};

const std::vector<Level> gameLevels = {
    {18, 10 *60, 360},  // Level 1
    {18, 8 * 60, 720},  // Level 2
    {24, 10 * 60, 1080},  // Level 3
    {24, 8 * 60, 1440},  // Level 4
    {36, 9 * 60, 1800},  // Level 5
    {36, 7 * 60, 2160},  // Level 6
};

#endif // LEVEL_H_INCLUDED
