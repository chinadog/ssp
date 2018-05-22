#ifndef SCREENMAINMENU_H
#define SCREENMAINMENU_H

#include "Screen/Screen.h"
#include <irrString.h>

class ScreenMainMenu : public Screen
{
public:
    ScreenMainMenu(GamePark* gamePark = 0);

    void draw();
private:
    irr::core::stringw str;
};

#endif // SCREENMAINMENU_H
