#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <irrlicht.h>
#include <UI/StaticText.h>

using namespace irr;

class MessageBox
{
public:
    MessageBox();

    static void showStartMessage(IrrlichtDevice* device);
};

#endif // MESSAGEBOX_H
