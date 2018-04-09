#include "MessageBox.h"

MessageBox::MessageBox()
{

}

void MessageBox::showStartMessage(IrrlichtDevice* device)
{
    core::dimension2di topLeft;
    core::dimension2di bottomRight;

    topLeft.set(device->getVideoDriver()->getScreenSize().Width/3*1,
                device->getVideoDriver()->getScreenSize().Height/3*1-20);
    bottomRight.set(device->getVideoDriver()->getScreenSize().Width/3*2,
                device->getVideoDriver()->getScreenSize().Height/3*1+20);

//    core::stringc text;
    bool border = false;
    bool wordWrap = true;
    bool background = true;
    core::rect<s32> rect(topLeft.Width,topLeft.Height,
                         bottomRight.Width,bottomRight.Height);
    const wchar_t* text = L"Спасательный шатл ждет в заливе на северо-западе парка.";

    StaticText* msg = new StaticText(device, text, border, device->getGUIEnvironment(), device->getGUIEnvironment()->getRootGUIElement(), -1, rect, background);
    msg->drop();
    msg->setWordWrap( wordWrap );
    msg->setLifeTime(1500, 6000);
}
