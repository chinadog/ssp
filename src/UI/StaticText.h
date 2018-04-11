#ifndef STATICTEXT_H
#define STATICTEXT_H

#include "IrrCompileConfig.h"
#include "IGUIStaticText.h"
#include "irrArray.h"
#include "IrrlichtDevice.h"

using namespace irr;

class StaticText : public gui::IGUIStaticText
{
public:

    //! constructor
    StaticText(IrrlichtDevice* device,
            const wchar_t* text, bool border, gui::IGUIEnvironment* environment,
        IGUIElement* parent, s32 id, const core::rect<s32>& rectangle,
        bool background = false);

    //! destructor
    virtual ~StaticText();

    //! draws the element and its children
    virtual void draw();

    //! Sets another skin independent font.
    virtual void setOverrideFont(gui::IGUIFont* font=0);

    //! Gets the override font (if any)
    virtual gui::IGUIFont* getOverrideFont() const;

    //! Get the font which is used right now for drawing
    virtual gui::IGUIFont* getActiveFont() const;

    //! Sets another color for the text.
    virtual void setOverrideColor(video::SColor color);

    //! Sets another color for the background.
    virtual void setBackgroundColor(video::SColor color);

    //! Sets whether to draw the background
    virtual void setDrawBackground(bool draw);

    //! Gets the background color
    virtual video::SColor getBackgroundColor() const;

    //! Checks if background drawing is enabled
    virtual bool isDrawBackgroundEnabled() const;

    //! Sets whether to draw the border
    virtual void setDrawBorder(bool draw);

    //! Checks if border drawing is enabled
    virtual bool isDrawBorderEnabled() const;

    //! Sets alignment mode for text
    virtual void setTextAlignment(gui::EGUI_ALIGNMENT horizontal, gui::EGUI_ALIGNMENT vertical);

    //! Gets the override color
    virtual video::SColor getOverrideColor() const;

    //! Sets if the static text should use the overide color or the
    //! color in the gui skin.
    virtual void enableOverrideColor(bool enable);

    //! Checks if an override color is enabled
    virtual bool isOverrideColorEnabled() const;

    //! Set whether the text in this label should be clipped if it goes outside bounds
    virtual void setTextRestrainedInside(bool restrainedInside);

    //! Checks if the text in this label should be clipped if it goes outside bounds
    virtual bool isTextRestrainedInside() const;

    //! Enables or disables word wrap for using the static text as
    //! multiline text control.
    virtual void setWordWrap(bool enable);

    //! Checks if word wrap is enabled
    virtual bool isWordWrapEnabled() const;

    //! Sets the new caption of this element.
    virtual void setText(const wchar_t* text);

    //! Returns the height of the text in pixels when it is drawn.
    virtual s32 getTextHeight() const;

    //! Returns the width of the current text, in the current font
    virtual s32 getTextWidth() const;

    //! Updates the absolute position, splits text if word wrap is enabled
    virtual void updateAbsolutePosition();

    //! Set whether the string should be interpreted as right-to-left (RTL) text
    /** \note This component does not implement the Unicode bidi standard, the
    text of the component should be already RTL if you call this. The
    main difference when RTL is enabled is that the linebreaks for multiline
    elements are performed starting from the end.
    */
    virtual void setRightToLeft(bool rtl);

    //! Checks if the text should be interpreted as right-to-left text
    virtual bool isRightToLeft() const;

    //! Writes attributes of the element.
    virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const;

    //! Reads attributes of the element
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    void setLifeTime(u32 delay, u32 lifeTime);

private:

    //! Breaks the single text line.
    void breakText();

    gui::EGUI_ALIGNMENT HAlign, VAlign;
    bool Border;
    bool OverrideColorEnabled;
    bool OverrideBGColorEnabled;
    bool WordWrap;
    bool Background;
    bool RestrainTextInside;
    bool RightToLeft;

    video::SColor OverrideColor, BGColor;
    gui::IGUIFont* OverrideFont;
    gui::IGUIFont* LastBreakFont; // stored because: if skin changes, line break must be recalculated.

    core::array< core::stringw > BrokenText;

    IrrlichtDevice* m_device;
    u32 m_startTime;
    u32 m_endTime;
};

#endif // STATICTEXT_H