#include "Config.h"

Config::Config() :
    m_freeFly(false),
    m_fog(false)
{

}

bool Config::load(const std::string &filename)
{
    INIReader reader(filename);
    if(reader.parseError())
    {
        return false;
    }
    m_params.WindowSize.Height = reader.getInteger("general","height",600);
    m_params.WindowSize.Width = reader.getInteger("general","width",800);
    m_params.Fullscreen = reader.getBoolean("general","fullScreen",false);
    m_params.Vsync = reader.getBoolean("general","vSync",false);
    m_params.Stencilbuffer = reader.getBoolean("general","stencilBuffer",false);
    m_params.AntiAlias = reader.getInteger("general","antiAlias",8);
    setFreeFly( reader.getBoolean("general","freeFly",false) );
    setFog( reader.getBoolean("general","fog",false) );
    setNormals( reader.getBoolean("general","normals",false) );
    setCount( reader.getInteger("general","count",0) );
    player().setRunSpeed( reader.getReal("player","runSpeed",0.0) );
    player().setWalkSpeed( reader.getReal("player","walkSpeed",0.0) );
    monster().setAtackSpeed( reader.getReal("monster","atackSpeed",0.0) );
    monster().setWalkSpeed( reader.getReal("monster","walkSpeed",0.0) );
    respawn().setCount( reader.getInteger("respawn","count",0) );
    for(int i=0;i<respawn().count();i++)
    {
        Respawn::Point point(reader.get("respawn","point"+std::to_string(i+1),"0,0,0"));
        respawn().points().push_back(point);
    }
    return true;
}

