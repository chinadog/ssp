#ifndef INOUTFADER_H
#define INOUTFADER_H

#include "IrrCompileConfig.h"
#include "IGUIInOutFader.h"
#include "IrrlichtDevice.h"
#include "Common/SignalSlot.h"

using namespace irr;

class InOutFader : public gui::IGUIInOutFader
{
public:

    //! constructor
    InOutFader(IrrlichtDevice* dev,
            gui::IGUIEnvironment* environment, IGUIElement* parent,
        s32 id, core::rect<s32> rectangle);

    //! draws the element and its children
    virtual void draw();

    //! Gets the color to fade out to or to fade in from.
    virtual video::SColor getColor() const;

    //! Sets the color to fade out to or to fade in from.
    virtual void setColor(video::SColor color );
    virtual void setColor(video::SColor source, video::SColor dest);

    //! Starts the fade in process.
    virtual void fadeIn(u32 time);

    //! Starts the fade out process.
    virtual void fadeOut(u32 time);

    //! Returns if the fade in or out process is done.
    virtual bool isReady() const;

    //! Writes attributes of the element.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

    //! Reads attributes of the element
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    Signal<> finished;

private:

    enum EFadeAction
    {
        EFA_NOTHING = 0,
        EFA_FADE_IN,
        EFA_FADE_OUT
    };

    u32 StartTime;
    u32 EndTime;
    EFadeAction Action;

    video::SColor Color[2];
    video::SColor FullColor;
    video::SColor TransColor;

    IrrlichtDevice* m_device = nullptr;
};



#endif // INOUTFADER_H
