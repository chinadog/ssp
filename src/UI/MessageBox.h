#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <irrlicht.h>
#include <UI/StaticText.h>

using namespace irr;

class MyMessageBox
{
public:
    MyMessageBox();

    static void showStartMessage(IrrlichtDevice* device);
};

#endif // MESSAGEBOX_H
