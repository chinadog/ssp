#ifndef SCREENGAMEOVER_H
#define SCREENGAMEOVER_H

#include "Screen/Screen.h"

class ScreenGameOver : public Screen
{
public:
    ScreenGameOver(GamePark* gamePark);
    void draw();
};

#endif // SCREENGAMEOVER_H
