#ifndef SCREENLOADING_H
#define SCREENLOADING_H

#include "Screen/Screen.h"
#include <irrlicht.h>

using namespace irr;

class ScreenLoading : public Screen
{
public:
    ScreenLoading(GamePark* gamePark = 0);
    void draw();
private:
    gui::IGUIImage* image = nullptr;
    gui::IGUIStaticText* statusText = nullptr;

    void load();
};

#endif // SCREENLOADING_H
