#ifndef SCREENENDGAME_H
#define SCREENENDGAME_H

#include "Screen/Screen.h"

class ScreenEndGame : public Screen
{
public:
    ScreenEndGame(GamePark* gamePark = 0);
    void draw();
};

#endif // SCREENENDGAME_H
