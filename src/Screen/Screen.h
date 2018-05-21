#ifndef SCREEN_H
#define SCREEN_H

namespace irr
{
    namespace video
    {
        class IVideoDriver;
    }
    namespace scene
    {
        class ISceneManager;
    }
    namespace gui
    {
        class IGUIEnvironment;
    }
    class IrrlichtDevice;
}
class Player;
class GamePark;

class Screen
{
public:
    Screen(GamePark* gamePark = 0);
    virtual ~Screen();

    virtual void draw() = 0;
    virtual void type(){}
    void remove() {m_isRemove = true;}
    bool isRemove() {return m_isRemove;}
protected:
    GamePark* m_gamePark = nullptr;
    irr::video::IVideoDriver* m_driver = nullptr;
    irr::scene::ISceneManager* m_smgr = nullptr;
    irr::gui::IGUIEnvironment* m_env = nullptr;
    Player* m_player = nullptr;
    irr::IrrlichtDevice* m_device = nullptr;
    bool m_isRemove = false;
};

#endif // SCREEN_H
