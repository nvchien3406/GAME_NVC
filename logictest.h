#ifndef LOGICTEST_H_INCLUDED
#define LOGICTEST_H_INCLUDED

void xxx(){
    graphics.drawMap(pikachu.mp/*, pikachu.rows*/, pikachu, isPause);
    graphics.drawTime(graphics.renderer, font, remainingTime);
    graphics.drawScore(graphics.renderer, font, score);
    graphics.drawScore1(graphics.renderer, font1);
    graphics.drawLevel(graphics.renderer, font1);
    graphics.drawLevel1(graphics.renderer, font1, currentLevel + 1);
    graphics.presentScene();
}

#endif // LOGICTEST_H_INCLUDED
