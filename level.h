#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED
#include <vector>
struct Level{
    int imgcount;
    int timelimit;
};

const std::vector<Level> gameLevels = {
    {18, 10 * 60},  // Level 1
    {18,  8 * 60},  // Level 2
    {24, 10 * 60},  // Level 3
    {24,  8 * 60},  // Level 4
    {36, 10 * 60},  // Level 5
    {36,  8 * 60},  // Level 6
};

#endif // LEVEL_H_INCLUDED
